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
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "err.h"
#include "utils.h"


uint64_t htonll(uint64_t value) {
    static const int num = 42;
    if (*(const char *)&num == 42) {
        return ((uint64_t)htonl(value & 0xFFFFFFFF) << 32) | htonl(value >> 32);
    } else {
        return value;
    }
}

uint64_t ntohll(uint64_t value) {
    return htonll(value);
}

uint64_t current_time_millis(uint64_t start_time) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000 - start_time;
}

uint16_t read_port(char const *string) {
    char *endptr;
    errno = 0;
    unsigned long port = strtoul(string, &endptr, 10);
    if (errno != 0 || *endptr != 0 || port > UINT16_MAX) {
        fatal("%s is not a valid port number", string);
    }
    return (uint16_t) port;
}

struct sockaddr_in get_server_address(char const *host, uint16_t port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct sockaddr_in send_address;
    send_address.sin_family = AF_INET;   // IPv4
    if (host != NULL) {
        struct addrinfo *address_result;
        int errcode = getaddrinfo(host, NULL, &hints, &address_result);
        if (errcode != 0) {
            fatal("getaddrinfo: %s", gai_strerror(errcode));
        }
        send_address.sin_addr.s_addr =
            ((struct sockaddr_in *) (address_result->ai_addr))->sin_addr.s_addr;
        freeaddrinfo(address_result);
    } else {
        send_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    send_address.sin_port = htons(port); // port from the command line

    return send_address;
}