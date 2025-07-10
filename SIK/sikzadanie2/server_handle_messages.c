#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "server_handle_messages.h"
#include "server_utils.h"
#include "err.h"

void handle_hello(server_data *data, int player_id, char *player_name) {
    if (data->players[player_id].received_hello) {
        bad_message(inet_ntoa(data->players[player_id].addr.sin_addr), ntohs(data->players[player_id].addr.sin_port), 
                    data->players[player_id].player_name, player_name - strlen("HELLO "));
        return;
    }

    strncpy(data->players[player_id].player_name, player_name, BUFFER_SIZE - 1);
    // Ensure null termination and remove \r\n
    data->players[player_id].player_name[strlen(player_name) - 2] = '\0';
    if (!is_valid_player_id(data->players[player_id].player_name)) {
        strcpy(data->players[player_id].player_name, "UNKNOWN");
        bad_message(inet_ntoa(data->players[player_id].addr.sin_addr), ntohs(data->players[player_id].addr.sin_port), 
                    data->players[player_id].player_name, player_name - strlen("HELLO "));
        return;
    }
    data->players[player_id].received_hello = true;
    printf("Player %d connected with name: %s\n", player_id, data->players[player_id].player_name);

    char line[BUFFER_SIZE] = {0};
    if (!fgets(line, sizeof(line), data->file)) {
        cleanup_server_data(data);
        fatal("Failed to read coefficients from file");
    }
    read_doubles(line, data->players[player_id].coeffs);
    if (send_all(data->fds[player_id + 1].fd, line, strlen(line)) == -1) {
        cleanup_server_data(data);
        syserr("send");
    }
    printf("Sent coefficients to player %d: %s", player_id, line);
    data->players[player_id].time_of_next_guess = current_time_millis();
}

void handle_put(server_data *data, int player_id, char *message) {
    int point;
    double value;
    char* message_copy = strdup(message);
    if (parse_point_value(message, &point, &value) != 0) {
        bad_message(inet_ntoa(data->players[player_id].addr.sin_addr), ntohs(data->players[player_id].addr.sin_port), 
                    data->players[player_id].player_name, message_copy);
        free(message_copy);
        return;
    }
    printf("Player %d sent put (%d, %.7f)\n", player_id, point, value);
    if (!data->players[player_id].received_hello || current_time_millis() < data->players[player_id].time_of_next_guess) {
        printf("Player %d is trying to guess too soon, applying penalty.\n", player_id);
        data->players[player_id].score -= PENALTY_TOO_SOON;
        char penalty_message[BUFFER_SIZE];
        snprintf(penalty_message, sizeof(penalty_message), "PENALTY %s", message_copy);
        if (send_all(data->fds[player_id + 1].fd, penalty_message, strlen(penalty_message)) == -1) {
            cleanup_server_data(data);
            syserr("send");
        }        
        free(message_copy);
        return;
    }
    if (point < 0 || point > data->K || value < MIN_VALUE || value > MAX_VALUE) {
        printf("Player %d sent invalid put (%d, %.7f), applying penalty.\n", player_id, point, value);
        data->players[player_id].score -= PENALTY_BAD_PUT;
        char* penalty_message = malloc(strlen("BAD PUT ") + strlen(message_copy) + 1);
        if (!penalty_message) {
            cleanup_server_data(data);
            syserr("malloc");
        }
        snprintf(penalty_message, strlen("BAD PUT ") + strlen(message_copy) + 1, "BAD PUT %s", message_copy);
        schedule_message(data, player_id, current_time_millis() + DELAY_BAD_PUT, penalty_message);
        free(message_copy);
        return;
    }
    free(message_copy);
    data->players[player_id].approximation[point] += value;
    data->players[player_id].number_of_guesses++;
    data->number_of_guesses_total++;
    uint64_t delay = count_small_letters(data->players[player_id].player_name);
    data->players[player_id].time_of_next_guess = current_time_millis() + delay * 1000; // Convert to milliseconds
    char* state_message = malloc((data->K + 1) * DOUBLE_CHAR_SIZE + 20);
    if (!state_message) {
        cleanup_server_data(data);
        syserr("malloc");
    }
    strcpy(state_message, "STATE ");
    for (int i = 0; i <= data->K; i++) {
        snprintf(state_message + strlen(state_message), DOUBLE_CHAR_SIZE + 1, "%.7f ", data->players[player_id].approximation[i]);
    }
    strncat(state_message, "\r\n\0", 3);
    schedule_message(data, player_id, current_time_millis() + delay * 1000, state_message);
    if (data->number_of_guesses_total >= data->M) {
        printf("Total number of guesses reached %d, ending game.\n", data->M);
        end_game(data);
    } else {
        printf("Player %d (%s) made a guess at point %d with value %.7f\n", player_id, data->players[player_id].player_name, point, value);
    }
    // Scheduled messages will be freed when sent
}

void handle_message(server_data *data, int player_id, char *message) {
    if (strncmp(message, "HELLO ", strlen("HELLO ")) == 0) {
        handle_hello(data, player_id, message + strlen("HELLO "));
    } else if (strncmp(message, "PUT ", strlen("PUT ")) == 0) {
        handle_put(data, player_id, message + strlen("PUT "));
    } else {
        bad_message(inet_ntoa(data->players[player_id].addr.sin_addr), ntohs(data->players[player_id].addr.sin_port), 
                    data->players[player_id].player_name, message);
    }
}