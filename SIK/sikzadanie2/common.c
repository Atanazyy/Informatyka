#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "err.h"
#include "common.h"

uint16_t read_port(char const *string) {
    char *endptr;
    errno = 0;
    unsigned long port = strtoul(string, &endptr, 10);
    if (errno != 0 || *endptr != 0 || port > UINT16_MAX) {
        fatal("%s is not a valid port number", string);
    }
    return (uint16_t) port;
}

bool is_valid_player_id(const char *id) {
    for (size_t i = 0; i < strlen(id); ++i) {
        if (!isalnum((unsigned char)id[i])) return false;
    }
    return true;
}

bool is_valid_rational(const char *str) {
    const char *p = str;
    int digits_after_dot = 0;

    if (*p == '-') {
        p++;
    }

    int digits_before_dot = 0;

    while (isdigit(*p)) {
        digits_before_dot++;
        p++;
    }

    if (*p == '.') {
        p++;
        while (isdigit(*p)) {
            if (++digits_after_dot > 7)
                return false;
            p++;
        }
    }

    if (digits_before_dot + digits_after_dot == 0)
        return false;

    if (*p != '\0')
        return false;

    return true;
}

int read_to_buffer(int sockfd, char *buffer, int buffer_size, size_t *len) {
    ssize_t bytes_read = recv(sockfd, buffer + *len, buffer_size - *len, 0);
    if (bytes_read < 0) {
        syserr("recv");
    } else if (bytes_read == 0) {
        return -1; // Connection closed
    }
    *len += bytes_read;
    return 0;
}

// Extracts one full CRLF-terminated message
int get_next_message(char *buffer, size_t *buffer_len, char *output) {
    for (size_t i = 0; i + 1 < *buffer_len; ++i) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            size_t msg_len = i + 2;
            memcpy(output, buffer, msg_len);
            output[msg_len] = '\0'; // null-terminate for convenience
            memmove(buffer, buffer + msg_len, *buffer_len - msg_len);
            *buffer_len -= msg_len;
            return 0;
        }
    }
    return -1; // No complete message yet
}

u_int64_t current_time_millis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
}

int read_doubles(const char *line, double *doubles) {
    char *copy = strdup(line);
    if (!copy) {
        syserr("strdup");
    }
    char *token = strtok(copy, " \r\n");
    int count = 0;

    if (!token || (strcmp(token, "COEFF") != 0 && strcmp(token, "STATE") != 0)) {
        free(copy);
        return -1;
    }

    while ((token = strtok(NULL, " \r\n")) != NULL) {
        char *endptr;
        double val = strtod(token, &endptr);
        errno = 0;
        if (*endptr != '\0' || errno != 0 || !is_valid_rational(token)) {
            free(copy);
            return -1;
        }
        doubles[count++] = val;
    }
    if (strtok(NULL, " \r\n") != NULL) {
        free(copy);
        return -1;
    }

    free(copy);
    return 0;
}

int parse_point_value(char *line, int *point_out, double *value_out) {
    char *endptr;
    char *token1, *token2;
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    len = strlen(line);
    if (len > 0 && line[len - 1] == '\r') {
        line[len - 1] = '\0';
    }
    token1 = strtok(line, " ");
    token2 = strtok(NULL, " ");
    if (!token1 || !token2 || strtok(NULL, " ") != NULL) {
        return -1;
    }
    errno = 0;
    long point = strtol(token1, &endptr, 10);
    if (*endptr != '\0' || errno != 0) {
        return -1;
    }
    *point_out = (int)point;
    errno = 0;
    double value = strtod(token2, &endptr);
    if (*endptr != '\0' || errno != 0 || !is_valid_rational(token2)) {
        return -1;
    }
    *value_out = value;

    return 0;
}

int count_small_letters(const char *str) {
    int count = 0;
    while (*str) {
        if (islower((unsigned char)*str)) {
            count++;
        }
        str++;
    }
    return count;
}

int send_all(int sockfd, const char *msg, size_t len) {
    size_t total_sent = 0;
    while (total_sent < len) {
        ssize_t bytes_sent = send(sockfd, msg + total_sent, len - total_sent, 0);
        if (bytes_sent < 0) {
            return -1; // Error occurred
        }
        total_sent += bytes_sent;
    }
    return 0; // All data sent successfully
}

double evaluate_polynomial(const double *coeffs, int k) {
    double result = 0.0;
    double pow_k = 1.0;
    for (int i = 0; i <= MAX_N; i++) {
        result += coeffs[i] * pow_k;
        pow_k *= k;
    }
    return result;
}

int parse_int(const char *arg, int min, int max, const char *optname) {
    char *endptr = NULL;
    errno = 0;
    long val = strtol(arg, &endptr, 10);

    if (errno || *endptr != '\0' || val < min || val > max) {
        fprintf(stderr, "Error: Invalid value for %s: %s\n", optname, arg);
        exit(EXIT_FAILURE);
    }

    return (int)val;
}