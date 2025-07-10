#ifndef MIM_COMMON_H
#define MIM_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#define LARGE_BUFFER_SIZE 1000000
#define BUFFER_SIZE 1000
#define DOUBLE_CHAR_SIZE 20
#define MAX_PLAYER_ID_LENGTH 30
#define EPS 1e-7

#define MAX_K 10000
#define MAX_N 8
#define MAX_M 12341234

#define MIN_VALUE -5.0
#define MAX_VALUE 5.0

uint16_t read_port(char const *string);
size_t read_size(char const *string);

struct sockaddr_in get_server_address(char const *host, uint16_t port);

bool is_valid_player_id(const char *id);
bool is_valid_rational(const char *str);
int read_to_buffer(int sockfd, char *buffer, int buffer_size, size_t *len);
int get_next_message(char *buffer, size_t *buffer_len, char *msg);
uint64_t current_time_millis();
int read_doubles(const char *line, double *coeffs);
int parse_point_value(char *line, int *point_out, double *value_out);
int count_small_letters(const char *str);
int send_all(int sockfd, const char *msg, size_t len);
double evaluate_polynomial(const double *coeffs, int point);
int parse_int(const char *arg, int min, int max, const char *optname);

#endif
