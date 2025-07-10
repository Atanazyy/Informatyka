#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <stdbool.h>
#include "utils.h"

#define MES_HELLO 1
#define MES_HELLO_REPLY 2
#define MES_CONNECT 3
#define MES_ACK_CONNECT 4
#define MES_SYNC_START 11
#define MES_DELAY_REQUEST 12
#define MES_DELAY_RESPONSE 13
#define MES_LEADER 21
#define MES_GET_TIME 31
#define MES_TIME 32

void handle_hello(bool verbose, int socket_fd, struct sockaddr_in client_address, node_info known_nodes[], uint16_t* known_nodes_count);
void handle_connect(bool verbose, int socket_fd, struct sockaddr_in client_address, node_info known_nodes[], uint16_t* known_nodes_count);
void handle_get_time(bool verbose, int socket_fd, struct sockaddr_in client_address, int64_t offset, uint8_t sync_lvl, uint64_t start_time);
void handle_delay_request(bool verbose, int socket_fd, struct sockaddr_in client_address, int64_t offset, uint8_t sync_lvl, uint64_t start_time);
void handle_sync_start(bool verbose, int socket_fd, struct sockaddr_in client_address, data message, node_info known_nodes[], uint16_t* known_nodes_count,
                        uint8_t* sync_lvl, node_info* sync_node, sync_info* sync_process, uint64_t start_time, uint64_t* last_sync_time);
void handle_delay_response(bool verbose, struct sockaddr_in client_address, data message, uint8_t* sync_lvl, node_info* sync_node, 
                            sync_info* sync_process, uint64_t start_time, int64_t* offset, uint64_t* last_sync_time);
#endif