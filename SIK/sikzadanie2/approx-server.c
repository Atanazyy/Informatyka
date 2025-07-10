#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"
#include "err.h"
#include "server_utils.h"
#include "server_handle_messages.h"

int main(int argc, char *argv[]) {
    server_data* data = malloc(sizeof(server_data));
    if (!data) {
        syserr("malloc");
    }
    for (int i = 0; i < MAX_SCHEDULED_MESSAGES; i++) {
        data->scheduled_messages[i].player_id = -1;
    }
    for (int i = 0; i <= MAX_CLIENTS; i++) {
        data->fds[i].fd = -1;
    }

    parse_args(argc, argv, data);
    setup_server(data);

    char received_message[BUFFER_SIZE] = {0};

    while (1) {
        int ready = poll(data->fds, data->nfds, POLL_TIMEOUT);
        if (ready == -1 && errno != EINTR && errno != EAGAIN) {
            cleanup_server_data(data);
            syserr("poll");
        }

        if (data->fds[0].revents & POLLIN) {
            int new_fd = accept(data->fds[0].fd, NULL, NULL);
            if (new_fd < 0) {
                cleanup_server_data(data);
                syserr("accept");
            }
            // Find an empty slot for the new client
            if (new_fd >= 0) {
                for (int i = 1; i <= MAX_CLIENTS; i++) {
                    if (data->fds[i].fd == -1) {
                        data->fds[i].fd = new_fd;
                        data->fds[i].events = POLLIN;
                        data->fds[i].revents = 0;
                        if (i >= data->nfds) data->nfds = i + 1;
                        accept_client(data, i - 1); // Initialize player data
                        struct sockaddr_in client_addr;
                        socklen_t addr_len = sizeof(client_addr);
                        if (getpeername(new_fd, (struct sockaddr*)&client_addr, &addr_len) < 0) {
                            cleanup_server_data(data);
                            syserr("getpeername");
                        }
                        data->players[i - 1].addr = client_addr;
                        printf("Client %d connected.\n", i - 1);
                        break;
                    }
                }
            }
        }

        for (int i = 1; i < data->nfds; i++) {
            if (data->fds[i].fd == -1) continue;
            if (data->fds[i].revents & POLLIN) {
                if(read_to_buffer(data->fds[i].fd, data->players[i - 1].receive_buffer, BUFFER_SIZE, &data->players[i - 1].receive_buffer_len) < 0) {
                    printf("Client %d disconnected.\n", i - 1);
                    close(data->fds[i].fd);
                    data->fds[i].fd = -1;
                    data->number_of_guesses_total -= data->players[i - 1].number_of_guesses;
                } else {
                    // Process all messages in the receive buffer
                    while (get_next_message(data->players[i - 1].receive_buffer, &data->players[i - 1].receive_buffer_len, received_message) >= 0) {
                        handle_message(data, i - 1, received_message);
                    }
                }
            }
        }
        for (int i = 1; i < data->nfds; i++) {
            if (data->fds[i].fd == -1) continue;
            if (!data->players[i - 1].received_hello && current_time_millis() - data->players[i - 1].time_of_connection > TIME_TO_SAY_HELLO) {
                printf("Client %d didn't send HELLO in time, disconnecting.\n", i - 1);
                close(data->fds[i].fd);
                data->fds[i].fd = -1;
            }
        }

        send_scheduled_messages(data);
    }

    cleanup_server_data(data);
    return 0;    
}
