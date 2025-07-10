#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "message_handler.h"
#include "err.h"
#include "utils.h"

void handle_hello(bool verbose, int socket_fd, struct sockaddr_in client_address, node_info known_nodes[], uint16_t* known_nodes_count) {
    info(verbose, "Received hello message from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    data *mes_to_send = malloc(sizeof(data) + sizeof(node_info) * (*known_nodes_count));
    if (!mes_to_send) {
        syserr("malloc");
    }
    mes_to_send->message = MES_HELLO_REPLY;
    mes_to_send->count = htons((*known_nodes_count));
    for (int i = 0; i < (*known_nodes_count); i++) {
        mes_to_send->nodes[i].peer_port = htons(known_nodes[i].peer_port);
        mes_to_send->nodes[i].peer_address_length = known_nodes[i].peer_address_length;
        memcpy(mes_to_send->nodes[i].peer_address, known_nodes[i].peer_address, known_nodes[i].peer_address_length);
    }

    int send_flags = 0;
    ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data) + sizeof(node_info) * (*known_nodes_count), send_flags,
                                (struct sockaddr *) &client_address, sizeof(client_address));
    if (sent_length != (ssize_t)(sizeof(data) + sizeof(node_info) * (*known_nodes_count))) {
        free(mes_to_send);
        info(verbose, "Sending failed");
        return;
    }
    info(verbose, "Sent hello reply to %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    free(mes_to_send);
    node_info new_node;
    new_node.peer_address_length = sizeof(client_address.sin_addr);
    memcpy(new_node.peer_address, &client_address.sin_addr, new_node.peer_address_length);
    new_node.peer_port = ntohs(client_address.sin_port);
    if ((*known_nodes_count) >= MAX_NODES) {
        info(verbose, "Maximum number of nodes reached");
    }
    else {
        known_nodes[(*known_nodes_count)++] = new_node;
    }
    
    info(verbose, "Known nodes:");
    for (int i = 0; i < (*known_nodes_count); i++) {
        info(verbose, "Node %d: %s:%d", i + 1, inet_ntoa(*(struct in_addr*)known_nodes[i].peer_address), known_nodes[i].peer_port);
    }
}

void handle_connect(bool verbose, int socket_fd, struct sockaddr_in client_address, node_info known_nodes[], uint16_t (*known_nodes_count)) {
    info(verbose, "Received CONNECT from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    data *mes_to_send = malloc(sizeof(data));
    if (!mes_to_send) {
        syserr("malloc");
    }
    mes_to_send->message = MES_ACK_CONNECT;
    int send_flags = 0;
    ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data), send_flags,
                                (struct sockaddr *) &client_address, sizeof(client_address));
    if (sent_length != sizeof(data)) {
        free(mes_to_send);
        info(verbose, "Sending failed");
        return;
    }
    info(verbose, "Sent CONNECT_ACK to %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    free(mes_to_send);

    node_info new_node;
    new_node.peer_address_length = sizeof(client_address.sin_addr);
    memcpy(new_node.peer_address, &client_address.sin_addr, new_node.peer_address_length);
    new_node.peer_port = ntohs(client_address.sin_port);
    if ((*known_nodes_count) >= MAX_NODES) {
        info(verbose, "Maximum number of nodes reached");
    }
    else {
        known_nodes[(*known_nodes_count)++] = new_node;
    }
    info(verbose, "Known nodes:");
    for (int i = 0; i < (*known_nodes_count); i++) {
        info(verbose, "Node %d: %s:%d", i + 1, inet_ntoa(*(struct in_addr*)known_nodes[i].peer_address), known_nodes[i].peer_port);
    }
}

void handle_get_time(bool verbose, int socket_fd, struct sockaddr_in client_address, int64_t offset, uint8_t sync_lvl, uint64_t start_time) {
    info(verbose, "Received GET_TIME from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    data *mes_to_send = malloc(sizeof(data));
    if (!mes_to_send) {
        syserr("malloc");
    }
    mes_to_send->message = MES_TIME;
    mes_to_send->synchronized = sync_lvl;
    mes_to_send->timestamp = htonll(current_time_millis(start_time) - offset);

    int send_flags = 0;
    ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data), send_flags,
                                (struct sockaddr *) &client_address, sizeof(client_address));
    if (sent_length != sizeof(data)) {
        free(mes_to_send);
        info(verbose, "Sending failed");
        return;
    }
    info(verbose, "Sent TIME to %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    free(mes_to_send);
}

void handle_delay_request(bool verbose, int socket_fd, struct sockaddr_in client_address, int64_t offset, uint8_t sync_lvl, uint64_t start_time) {
    info(verbose, "Received DELAY_REQUEST from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    data *mes_to_send = malloc(sizeof(data));
    if (!mes_to_send) {
        syserr("malloc");
    }
    mes_to_send->message = MES_DELAY_RESPONSE;
    mes_to_send->synchronized = sync_lvl;
    mes_to_send->timestamp = htonll(current_time_millis(start_time) - offset);

    int send_flags = 0;
    ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data), send_flags,
                                (struct sockaddr *) &client_address, sizeof(client_address));
    if (sent_length != sizeof(data)) {
        free(mes_to_send);
        info(verbose, "Sending failed");
        return;
    }
    info(verbose, "Sent DELAY_RESPONSE to %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    free(mes_to_send);
}

void handle_sync_start(bool verbose, int socket_fd, struct sockaddr_in client_address, data message, node_info known_nodes[], uint16_t* known_nodes_count, 
                        uint8_t* sync_lvl, node_info* sync_node, sync_info* sync_process, uint64_t start_time, uint64_t* last_sync_time) {
    info(verbose, "Received SYNC_START from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    uint64_t recv_time = current_time_millis(start_time);
    info(verbose, "Sync level: %d", *sync_lvl);

    if (memcmp(&client_address.sin_addr, sync_node->peer_address, sync_node->peer_address_length) != 0 ||
        ntohs(client_address.sin_port) != sync_node->peer_port) {
        if (message.synchronized + 1 >= *sync_lvl) {
            info(verbose, "Ignoring SYNC_START from not synced node, synchronized too high");
            return;
        }
    }
    else if (message.synchronized >= *sync_lvl) {
        info(verbose, "Resetting sync level, synchronized node sync level too high");
        *sync_lvl = MAX_SYNC_LVL;
        *last_sync_time = 0;
        return;
    }
    else {
       *last_sync_time = recv_time;
    }
    if (message.synchronized >=  MAX_SYNC_LVL - 1) {
        info(verbose, "Ignoring SYNC_START, synchronized too high");
        return;
    }
    if (sync_process->waiting_for_message != MES_SYNC_START) {
        info(verbose, "Ignoring SYNC_START, sync process already in progress");
        return;
    }
    int node_id = -1;
    for (int i = 0; i < (*known_nodes_count); i++) {
        if (memcmp(&client_address.sin_addr, known_nodes[i].peer_address, known_nodes[i].peer_address_length) == 0 &&
            ntohs(client_address.sin_port) == known_nodes[i].peer_port) {
            node_id = i;
        }
    }
    if (node_id == -1) {
        info(verbose, "Unknown node %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        error_msg(&message);
        return;
    }
    data *mes_to_send = malloc(sizeof(data));
    if (!mes_to_send) {
        syserr("malloc");
    }
    mes_to_send->message = MES_DELAY_REQUEST;
    sync_process->node = known_nodes[node_id];
    sync_process->waiting_for_message = MES_DELAY_REQUEST;
    sync_process->synchronized = message.synchronized;
    sync_process->T1 = ntohll(message.timestamp);
    sync_process->T2 = recv_time;
    ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data), 0,
                                (struct sockaddr *) &client_address, sizeof(client_address));
    sync_process->T3 = current_time_millis(start_time);
    if (sent_length != sizeof(data)) {
        free(mes_to_send);
        info(verbose, "Sending failed");
        sync_process->waiting_for_message = MES_SYNC_START;
        return;
    }
    free(mes_to_send);
    info(verbose, "Sent DELAY_REQUEST to %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
}

void handle_delay_response(bool verbose, struct sockaddr_in client_address, data message, uint8_t* sync_lvl, node_info* sync_node, sync_info* sync_process, 
                            uint64_t start_time, int64_t* offset, uint64_t* last_sync_time) {
    info(verbose, "Received DELAY_RESPONSE from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    if (memcmp(&client_address.sin_addr, sync_process->node.peer_address, sync_process->node.peer_address_length) != 0 ||
        ntohs(client_address.sin_port) != sync_process->node.peer_port) {
        info(verbose, "Ignoring DELAY_RESPONSE, not from node we are syncing with");
        error_msg(&message);
        return;
    }
    if (message.synchronized != sync_process->synchronized) {
        info(verbose, "Ignoring DELAY_RESPONSE, sync level mismatch");
        error_msg(&message);
        return;
    }
    if (sync_process->waiting_for_message != MES_DELAY_REQUEST) {
        info(verbose, "Ignoring DELAY_RESPONSE, sync process already in different state");
        error_msg(&message);
        return;
    }
    sync_process->waiting_for_message = MES_SYNC_START;
    sync_process->T4 = ntohll(message.timestamp);
    int64_t new_offset = ((int64_t)sync_process->T2 - (int64_t)sync_process->T1 + (int64_t)sync_process->T3 - (int64_t)sync_process->T4) / 2;
    *sync_node = sync_process->node;
    *offset = new_offset;
    sync_process->waiting_for_message = MES_SYNC_START;
    *sync_lvl = sync_process->synchronized + 1;
    *last_sync_time = current_time_millis(start_time);
    info(verbose, "Synchronized with %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    info(verbose, "Offset: %" PRId64, new_offset);
    info(verbose, "Synchronized level: %d", *sync_lvl);
}