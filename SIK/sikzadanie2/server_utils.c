#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <sys/time.h>

#include "server_utils.h"
#include "common.h"
#include "err.h"

void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s [-p port] [-k K] [-n N] [-m M] -f file\n", progname);
}

// Sets up the server data structure, initializes file, and prepares the listening socket.
void setup_server(server_data *data) {
    data->number_of_guesses_total = 0;

    data->file = fopen(data->filename, "r");
    if (!data->file) {
        cleanup_server_data(data);
        syserr("fopen");
    }

    int listen_fd;
    struct sockaddr_in addr;
    data->nfds = 1; 

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        cleanup_server_data(data);
        syserr("socket");
    }

    if (memset(&addr, 0, sizeof(addr)) == NULL) {
        cleanup_server_data(data);
        syserr("memset");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(data->port);
    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cleanup_server_data(data);
        syserr("bind");
    }
    
    if (listen(listen_fd, SOMAXCONN) < 0) {
        cleanup_server_data(data);
        syserr("listen");
    }

    if (data->port == 0) {
        socklen_t addr_len = sizeof(addr);
        if (getsockname(listen_fd, (struct sockaddr*)&addr, &addr_len) < 0) {
            cleanup_server_data(data);
            syserr("getsockname");
        }
        data->port = ntohs(addr.sin_port);
    }
    printf("Server listening on %s:%d\n", inet_ntoa(addr.sin_addr), data->port);
    data->fds[0].fd = listen_fd;
    data->fds[0].events = POLLIN;
}

void schedule_message(server_data *data, int player_id, uint64_t time_of_sending, char *message) {
    for (int i = 0; i < MAX_SCHEDULED_MESSAGES; i++) {
        if (data->scheduled_messages[i].player_id == -1 || strncmp(data->scheduled_messages[i].player_name, data->players[player_id].player_name, BUFFER_SIZE) == 0) {
            strncpy(data->scheduled_messages[i].player_name, data->players[player_id].player_name, BUFFER_SIZE - 1);
            data->scheduled_messages[i].message = message;
            data->scheduled_messages[i].player_id = player_id;
            data->scheduled_messages[i].time_of_sending = time_of_sending;
            printf("Scheduled message for player %d: %s at time %llu\n", player_id, data->scheduled_messages[i].message, (unsigned long long)time_of_sending);
            return;
        }
    }
    fprintf(stderr, "ERROR: No space to schedule message for player %d\n", player_id);
}

void send_scheduled_messages(server_data *data) {
    uint64_t current_time = current_time_millis();
    for (int i = 0; i < MAX_SCHEDULED_MESSAGES; i++) {
        if (data->scheduled_messages[i].player_id != -1 && data->scheduled_messages[i].time_of_sending <= current_time) {
            int player_id = data->scheduled_messages[i].player_id;
            if (strcmp(data->scheduled_messages[i].player_name, data->players[player_id].player_name) != 0) {
                // Player name changed, so player has disconnected
                printf("Player %d has disconnected, removing scheduled message: %s\n", data->scheduled_messages[i].player_id, data->scheduled_messages[i].message);
                data->scheduled_messages[i].player_id = -1;
                free(data->scheduled_messages[i].message);
                data->scheduled_messages[i].message = NULL;
                continue;         
            }
            if (send_all(data->fds[player_id + 1].fd, data->scheduled_messages[i].message, strlen(data->scheduled_messages[i].message)) == -1) {
                cleanup_server_data(data);
                syserr("send");
            }
            printf("Sent scheduled message to player %d: %s\n", player_id, data->scheduled_messages[i].message);
            data->scheduled_messages[i].player_id = -1;
            free(data->scheduled_messages[i].message);
            data->scheduled_messages[i].message = NULL;
        }
    }
}

void parse_args(int argc, char *argv[], server_data *data) {
    data->port = -1;
    data->K = -1;
    data->N = -1;
    data->M = -1;
    data->filename = NULL;
    data->file = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "p:k:n:m:f:")) != -1) {
        switch (opt) {
            case 'p':
                if (data->port != -1) {
                    fprintf(stderr, "Error: Port already specified.\n");
                    print_usage(argv[0]);
                    cleanup_server_data(data);
                    exit(EXIT_FAILURE);
                }
                data->port = read_port(optarg);
                break;
            case 'k':
                if (data->K != -1) {
                    fprintf(stderr, "Error: K already specified.\n");
                    print_usage(argv[0]);
                    cleanup_server_data(data);
                    exit(EXIT_FAILURE);
                }
                data->K = parse_int(optarg, 1, MAX_K, "-k");
                break;
            case 'n':
                if (data->N != -1) {
                    fprintf(stderr, "Error: N already specified.\n");
                    print_usage(argv[0]);
                    cleanup_server_data(data);
                    exit(EXIT_FAILURE);
                }
                data->N = parse_int(optarg, 1, MAX_N, "-n");
                break;
            case 'm':
                if (data->M != -1) {
                    fprintf(stderr, "Error: M already specified.\n");
                    print_usage(argv[0]);
                    cleanup_server_data(data);
                    exit(EXIT_FAILURE);
                }
                data->M = parse_int(optarg, 1, MAX_M, "-m");
                break;
            case 'f':
                data->filename = strdup(optarg);
                if (!data->filename) {
                    cleanup_server_data(data);
                    syserr("strdup");
                }
                break;
            default:
                print_usage(argv[0]);
                cleanup_server_data(data);
                exit(EXIT_FAILURE);
        }
    }
    if (!data->filename) {
        fprintf(stderr, "Error: Missing required parameter: -f file\n");
        print_usage(argv[0]);
        cleanup_server_data(data);
        exit(EXIT_FAILURE);
    }
    if (data->port == -1) data->port = 0;
    if (data->K == -1) data->K = DEFAULT_K;
    if (data->N == -1) data->N = DEFAULT_N;
    if (data->M == -1) data->M = DEFAULT_M;
}

void accept_client(server_data *data, int player_id) {
    data->players[player_id].time_of_connection = current_time_millis();
    data->players[player_id].time_of_next_guess = 0;
    data->players[player_id].received_hello = false;
    strcpy(data->players[player_id].player_name, "UNKNOWN");
    data->players[player_id].player_id = player_id;
    data->players[player_id].number_of_guesses = 0;
    for (int i = 0; i <= MAX_N; i++) {
        data->players[player_id].coeffs[i] = 0.0;
    }
    for (int i = 0; i <= MAX_K; i++) {
        data->players[player_id].approximation[i] = 0.0;
    }
    data->players[player_id].score = 0.0;
    data->players[player_id].receive_buffer_len = 0;
    data->players[player_id].receive_buffer[0] = '\0';
}

// End game and prepare for next game.
void end_game(server_data *data) {
    char* end_message = malloc(MAX_CLIENTS * (DOUBLE_CHAR_SIZE + MAX_PLAYER_ID_LENGTH + 5) + 20);
    if (!end_message) {
        cleanup_server_data(data);
        syserr("malloc");
    }
    strcpy(end_message, "SCORING ");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (data->fds[i + 1].fd == -1) {
            continue;
        }
        for (int k = 0; k <= data->K; k++) {
            double error = (data->players[i].approximation[k] - evaluate_polynomial(data->players[i].coeffs, k));
            data->players[i].score += error * error;
        }
        printf("Player %d (%s) final score: %.7f\n", i, data->players[i].player_name, data->players[i].score);
        snprintf(end_message + strlen(end_message), DOUBLE_CHAR_SIZE * 2 + 50, "%s %.7f ", data->players[i].player_name, data->players[i].score);
    }
    strncat(end_message, "\r\n\0", 3);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (data->fds[i + 1].fd != -1) {
            if (send_all(data->fds[i + 1].fd, end_message, strlen(end_message)) == -1) {
                cleanup_server_data(data);
                free(end_message);
                syserr("send");
            }        
        }
    }
    free(end_message);
    printf("Game ended.\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (data->fds[i + 1].fd != -1) {
            close(data->fds[i + 1].fd);
            data->fds[i + 1].fd = -1;
        }
    }
    data->nfds = 1;
    data->number_of_guesses_total = 0;
    for (int i = 0; i < MAX_SCHEDULED_MESSAGES; i++) {
        if (data->scheduled_messages[i].player_id != -1) {
            free(data->scheduled_messages[i].message);
            data->scheduled_messages[i].message = NULL;
        }
        data->scheduled_messages[i].player_id = -1;
        data->scheduled_messages[i].time_of_sending = 0;
        data->scheduled_messages[i].player_name[0] = '\0';
    }
}

// Cleans up the server data structure, closes files, and frees allocated memory.
void cleanup_server_data(server_data *data) {

    if (data->filename) {
        free(data->filename);
    }
    if (data->file) {
        fclose(data->file);
    }
    for (int i = 0; i < MAX_SCHEDULED_MESSAGES; i++) {
        if (data->scheduled_messages[i].player_id != -1) {
            free(data->scheduled_messages[i].message);
        }
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (data->fds[i + 1].fd != -1) {
            close(data->fds[i + 1].fd);
            data->fds[i + 1].fd = -1;
        }
    }
    free(data);
}