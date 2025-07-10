#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <math.h>

#include "common.h"
#include "err.h"

void prepare_put(char *msg, double* coeffs, double* state) {
    for (int i = 0; i <= MAX_K; i++) {
        double target = evaluate_polynomial(coeffs, i);
        if (fabs(target - state[i]) > EPS) {
            double value = target - state[i];
            if (value < MIN_VALUE) {
                value = MIN_VALUE;
            } else if (value > MAX_VALUE) {
                value = MAX_VALUE;
            }
            snprintf(msg, BUFFER_SIZE, "PUT %d %.7f\r\n", i, value);
            return;
        }
    }
    snprintf(msg, BUFFER_SIZE, "PUT 0 0\r\n");
}

void print_usage(const char *prog_name) {
    fprintf(stderr,
        "Usage: %s -u player_id -s server -p port [-4|-6] [-a]\n"
        "  -u player_id : required, alphanumeric only\n"
        "  -s server    : required, hostname or IP\n"
        "  -p port      : required, number from 1 to 65535\n"
        "  -4           : optional, force IPv4\n"
        "  -6           : optional, force IPv6\n"
        "  -a           : optional, enable alternate strategy\n", prog_name);
}

int main(int argc, char *argv[]) {
    char* player_id = NULL;
    char* server = NULL;
    int port = -1;
    int ip_version = 0; // 0 = auto, 4 = IPv4, 6 = IPv6
    bool strategy_a = false;

    int opt;
    bool ipv4_set = false, ipv6_set = false;

    while ((opt = getopt(argc, argv, "u:s:p:46a")) != -1) {
        switch (opt) {
            case 'u':
                if (player_id != NULL) {
                    fatal("Error: player_id already set.");
                }
                if (!is_valid_player_id(optarg)) {
                    fatal("Error: Invalid player_id format.");
                }
                player_id = optarg;
                break;
            case 's':
                if (server != NULL) {
                    fatal("Error: Server already specified.");
                }
                server = optarg;
                break;
            case 'p': {
                if (port != -1) {
                    fatal("Error: Port already specified.");
                }
                port = read_port(optarg);
                break;
            }
            case '4':
                ipv4_set = true;
                break;
            case '6':
                ipv6_set = true;
                break;
            case 'a':
                strategy_a = true;
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (player_id == NULL || server == NULL || player_id[0] == '\0' || server[0] == '\0' || port == -1) {
        fprintf(stderr, "Error: Missing required arguments.\n");
        print_usage(argv[0]);
        return 1;
    } else if (port < 1 || port > 65535) {
        fatal("Port must be between 1 and 65535.");
    }

    if (ipv4_set && ipv6_set) {
        ip_version = 0; // auto-detect
    } else if (ipv4_set) {
        ip_version = 4;
    } else if (ipv6_set) {
        ip_version = 6;
    } else {
        ip_version = 0; // auto-detect
    }
    
    struct addrinfo hints, *res, *rp;
    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = (ip_version == 4) ? AF_INET :
                      (ip_version == 6) ? AF_INET6 : AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int ret = getaddrinfo(server, port_str, &hints, &res);
    if (ret != 0) {
        fatal("getaddrinfo: %s\n", gai_strerror(ret));
    }
    int sockfd = -1;
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) continue;
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            printf("Connected to server %s on port %d\n", server, port);
            break;
        }
        close(sockfd);
        sockfd = -1;
    }
    freeaddrinfo(res);
    if (sockfd == -1) {
        fatal("Unable to connect to server.");
    }
    
    char msg[BUFFER_SIZE];
    snprintf(msg, sizeof(msg), "HELLO %s\r\n", player_id);
    if (send_all(sockfd, msg, strlen(msg)) == -1) {
        close(sockfd);
        syserr("send");
    }
    printf("Sent message: %s", msg);

    char* buffer = malloc(LARGE_BUFFER_SIZE);
    if (!buffer) {
        close(sockfd);
        syserr("malloc");
    }
    char* received_msg = malloc(LARGE_BUFFER_SIZE);
    if (!received_msg) {
        free(buffer);
        close(sockfd);
        syserr("malloc");
    }
    size_t buffer_len = 0;
    buffer[0] = received_msg[0] = '\0';

    do {
        if (read_to_buffer(sockfd, buffer, LARGE_BUFFER_SIZE ,&buffer_len) < 0) {
            close(sockfd);
            fatal("Connection closed by server.");
        }
    } while (get_next_message(buffer, &buffer_len, received_msg) < 0);
    
    double coeffs[MAX_N + 1] = {0};
    double* state = malloc((MAX_K + 1) * sizeof(double));
    for (int i = 0; i <= MAX_K; i++) {
        state[i] = 0.0;
    }
    if (read_doubles(received_msg, coeffs) < 0) {
        close(sockfd);
        bad_message(server, port, player_id, received_msg);
        free(buffer);
        free(received_msg);
        fatal("Invalid coefficients received from server.");
    }

    printf("Received coefficients:\n");
    for (int i = 0; i <= MAX_N; i++) {
        printf("x^%d: %f\n", i, coeffs[i]);
    }

    if (strategy_a) {
        prepare_put(msg, coeffs, state);
        if (send_all(sockfd, msg, strlen(msg)) == -1) {
            close(sockfd);
            free(buffer);
            free(received_msg);
            free(state);
            syserr("send");
        }
        printf("Sent message: %s", msg);
    }

    struct pollfd fds[2];

    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;
    fds[1].revents = 0;

    while (1) {
        if (fds[0].revents & POLLIN) {
            if (read_to_buffer(sockfd, buffer, LARGE_BUFFER_SIZE, &buffer_len) < 0) {
                close(sockfd);
                free(buffer);
                free(received_msg);
                free(state);
                fatal("unexpected server disconnect");
            }
        }

        if (fds[1].revents & POLLIN & !strategy_a) {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("End of input.\n");
            }
            printf("[USER] %s", buffer);
            char *line_copy = strdup(buffer);
            if (!line_copy) {
                close(sockfd);
                free(buffer);
                free(received_msg);
                syserr("strdup");
            }
            int point;
            double value;
            if (parse_point_value(buffer, &point, &value) == 0) {
                snprintf(msg, sizeof(msg), "PUT %d %.7f\r\n", point, value);
                free(line_copy);
                if (send_all(sockfd, msg, strlen(msg)) == -1) {
                    close(sockfd);
                    free(buffer);
                    free(received_msg);
                    syserr("send");
                }
                printf("Sent message: %s", msg);
            } else {
                fprintf(stderr, "Invalid input line: %s", line_copy);
                free(line_copy);
            }
        }

        while (get_next_message(buffer, &buffer_len, received_msg) >= 0) {
            printf("[SERVER] %s", received_msg);
            if (strncmp(received_msg, "STATE ", 6) == 0 && strategy_a) {
                if (read_doubles(received_msg, state) < 0) {
                    bad_message(server, port, player_id, received_msg);
                    continue;
                }
                prepare_put(msg, coeffs, state);
                if (send_all(sockfd, msg, strlen(msg)) == -1) {
                    close(sockfd);
                    free(buffer);
                    free(received_msg);
                    free(state);
                    syserr("send");
                }
                printf("Sent message: %s", msg);
            }
            if (strncmp(received_msg, "SCORING", 7) == 0) {
                printf("Game ended by server.\n");
                free(buffer);
                free(received_msg);
                free(state);
                return 0;
            }
        }
        int ret = poll(fds, 2, -1);
        if (ret < 0) {
            close(sockfd);
            free(buffer);
            free(received_msg);
            syserr("poll");
            break;
        }
    }
    close(sockfd);
    return 0;
}