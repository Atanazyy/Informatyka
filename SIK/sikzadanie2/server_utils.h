#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#include "common.h"

#define DEFAULT_K 100
#define DEFAULT_N 4
#define DEFAULT_M 131

#define MAX_CLIENTS 100
#define MAX_SCHEDULED_MESSAGES 100
#define TIME_TO_SAY_HELLO 3000 // milliseconds
#define PENALTY_TOO_SOON 20
#define PENALTY_BAD_PUT 10
#define DELAY_BAD_PUT 1000 // milliseconds
#define POLL_TIMEOUT 100 // milliseconds

typedef struct {
    struct sockaddr_in addr;
    uint64_t time_of_connection;
    uint64_t time_of_next_guess;
    bool received_hello;
    char player_name[BUFFER_SIZE];
    int player_id;
    int number_of_guesses;
    double coeffs[MAX_N + 1];
    double approximation[MAX_K + 1];
    double score;
    char receive_buffer[BUFFER_SIZE];
    size_t receive_buffer_len;
} player_info;

typedef struct {
    char player_name[BUFFER_SIZE];
    char* message;
    int player_id;
    uint64_t time_of_sending;
} scheduled_message;

typedef struct {
    int port;
    int K;
    int N;
    int M;
    player_info players[MAX_CLIENTS];
    int number_of_guesses_total;
    struct pollfd fds[MAX_CLIENTS + 1];
    scheduled_message scheduled_messages[MAX_SCHEDULED_MESSAGES];
    int nfds;
    char *filename;
    FILE *file;
} server_data;

void setup_server(server_data *data);
void parse_args(int argc, char *argv[], server_data *data);
void accept_client(server_data *data, int player_id);
void send_scheduled_messages(server_data *data);
void cleanup_server_data(server_data *data);
void schedule_message(server_data *data, int player_id, uint64_t time_of_sending, char *message);
void end_game(server_data *data);

#endif