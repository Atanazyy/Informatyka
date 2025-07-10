#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>

#define MSG_GET_TIME 31
#define MSG_TIME 32

#define TIMEOUT_SEC 2

typedef struct __attribute__((__packed__)) {
    uint8_t message;
} get_time_msg_t;

typedef struct __attribute__((__packed__)) {
    uint8_t message;
    uint8_t synchronized;
    uint64_t timestamp;
} time_msg_t;

// Helper to convert network to host byte order for uint64_t
uint64_t ntohll(uint64_t val) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return (((uint64_t)ntohl(val & 0xFFFFFFFF)) << 32) | ntohl(val >> 32);
#else
    return val;
#endif
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return 1;
    }

    // Prepare address
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int err = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (err != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    // Set timeout for recvfrom
    struct timeval tv = {TIMEOUT_SEC, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // Prepare and send GET_TIME
    get_time_msg_t get_time;
    get_time.message = MSG_GET_TIME;
    ssize_t sent = sendto(sockfd, &get_time, sizeof(get_time), 0, res->ai_addr, res->ai_addrlen);
    if (sent != sizeof(get_time)) {
        perror("sendto");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    // Receive TIME
    time_msg_t time_msg;
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    ssize_t recvd = recvfrom(sockfd, &time_msg, sizeof(time_msg), 0, (struct sockaddr*)&from, &fromlen);
    if (recvd < 0) {
        perror("recvfrom");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    if (time_msg.message != MSG_TIME) {
        fprintf(stderr, "Unexpected message type: %u\n", time_msg.message);
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    printf("TIME message received:\n");
    printf("  synchronized: %u\n", time_msg.synchronized);
    uint64_t timestamp = ntohll(time_msg.timestamp);
    printf("  timestamp:    %lu.%03lu\n", (unsigned long)(timestamp / 1000), (unsigned long)(timestamp % 1000));

    close(sockfd);
    freeaddrinfo(res);
    return 0;
}