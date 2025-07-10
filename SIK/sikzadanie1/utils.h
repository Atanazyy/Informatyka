#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define MAX_NODES 65535
#define MAX_SYNC_LVL 255
#define SYNC_START_INTERVAL 5000 // in milliseconds
#define TIMEOUT 2 // in seconds
#define RESET_SYNC_INTERVAL 25000 // in milliseconds

typedef struct __attribute__((__packed__)) {
    uint8_t peer_address_length;
    uint8_t peer_address[4];
    uint16_t peer_port;

} node_info;

typedef struct __attribute__((__packed__)) {
    uint8_t message;
    uint8_t synchronized;
    uint64_t timestamp;
    uint16_t count;
    node_info nodes[];
} data;

typedef struct {
    node_info node;
    uint8_t waiting_for_message;
    uint8_t synchronized;
    uint64_t T1;
    uint64_t T2;
    uint64_t T3;
    uint64_t T4;
} sync_info;

uint16_t read_port(char const *string);
struct sockaddr_in get_server_address(char const *host, uint16_t port);
uint64_t current_time_millis(uint64_t start_time);
uint64_t htonll(uint64_t value);
uint64_t ntohll(uint64_t value);

#endif