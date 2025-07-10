#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "err.h"
#include "utils.h"
#include "message_handler.h"

void send_sync_start(bool verbose, int socket_fd, node_info known_nodes[], uint16_t* known_nodes_count, 
                    int64_t offset, uint64_t start_time, uint8_t sync_lvl) {
    if (sync_lvl >= MAX_SYNC_LVL - 1) {
        return;
    }
    info(verbose, "Sending sync start message to:");
    for (int i = 0; i < *known_nodes_count; i++) {
        info(verbose, "Node %d: %s:%d", i + 1, inet_ntoa(*(struct in_addr*)known_nodes[i].peer_address), known_nodes[i].peer_port);
        struct sockaddr_in new_node_address;
        new_node_address = get_server_address(inet_ntoa(*(struct in_addr*)known_nodes[i].peer_address), known_nodes[i].peer_port);
        data *mes_to_send = malloc(sizeof(data));
        if (!mes_to_send) {
            syserr("malloc");
        }
        mes_to_send->message = MES_SYNC_START;
        mes_to_send->synchronized = sync_lvl;
        mes_to_send->timestamp = htonll(current_time_millis(start_time) - offset);
        int send_flags = 0;
        ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data), send_flags,
                                    (struct sockaddr *) &new_node_address, sizeof(new_node_address));
        if (sent_length != sizeof(data)) {
            info(verbose, "Sending failed");
            free(mes_to_send);
            continue;
        }
        info(verbose, "Sent sync start message to %s:%d", inet_ntoa(new_node_address.sin_addr), ntohs(new_node_address.sin_port));
        free(mes_to_send);   
    }
}

int main(int argc, char *argv[]) {
    uint64_t start_time = current_time_millis(0);

    char *bind_address = NULL;
    int port = 0;
    char *peer_address = NULL;
    int peer_port = -1;
    bool verbose = false;

    int opt;
    while ((opt = getopt(argc, argv, "b:p:a:r:v")) != -1) {
        switch (opt) {
            case 'b':
                if (bind_address != NULL) {
                    fatal("Bind address already specified: %s", bind_address);
                }
                bind_address = optarg;
                break;
            case 'p':
                if (port != 0) {
                    fatal("Port already specified: %d", port);
                }
                port = read_port(optarg);
                break;
            case 'a':
                if (peer_address != NULL) {
                    fatal("Peer address already specified: %s", peer_address);
                }
                peer_address = optarg;
                break;
            case 'r':
                if (peer_port != -1) {
                    fatal("Peer port already specified: %d", peer_port);
                }
                peer_port = read_port(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            default:
                fatal("Usage: %s [-b bind_address] [-p port] [-a peer_address -r peer_port]", argv[0]);
        }
    }
    if ((peer_address != NULL && peer_port == -1) || (peer_address == NULL && peer_port != -1)) {
        fatal("Peer address and port must be specified together.");
    }

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        syserr("cannot create a socket");
    }
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;        
    timeout.tv_usec = 0;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        syserr("setsockopt");
    }
    struct sockaddr_in my_address_in = get_server_address(bind_address, port);
    if (bind(socket_fd, (struct sockaddr *) &my_address_in, (socklen_t) sizeof(my_address_in)) < 0) {
        syserr("bind");
    }

    node_info known_nodes[MAX_NODES];
    uint16_t known_nodes_count = 0;
    if (peer_address != NULL && peer_port != -1) {
        struct sockaddr_in peer_address_in = get_server_address(peer_address, peer_port);
        data* mes_to_send = malloc(sizeof(data));
        if (!mes_to_send) {
            syserr("malloc");
        }
        mes_to_send->message = MES_HELLO;
        int send_flags = 0;
        ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data), send_flags,
                                    (struct sockaddr *) &peer_address_in, sizeof(peer_address_in));
        if (sent_length < 0) {
            free(mes_to_send);
            syserr("sendto");
        }
        else if (sent_length != sizeof(data)) {
            free(mes_to_send);
            fatal("incomplete sending");
        }
        free(mes_to_send);
        info(verbose, "Sent hello message to %s:%d", peer_address, peer_port);

        data* mes_to_receive = malloc(sizeof(data) + sizeof(node_info) * MAX_NODES);
        if (!mes_to_receive) {
            syserr("malloc");
        }
        int flags = 0;
        struct sockaddr_in client_address;
        socklen_t address_length = (socklen_t) sizeof(client_address);
        while (true) {
            ssize_t received_length = recvfrom(socket_fd, mes_to_receive, sizeof(data) + sizeof(node_info) * MAX_NODES, flags,
                                        (struct sockaddr *) &client_address, &address_length);
            if (received_length < 0) {
                info(verbose, "Receive error");
                continue;
            }
            if (memcmp(&client_address.sin_addr, &peer_address_in.sin_addr, sizeof(client_address.sin_addr)) != 0 ||
                ntohs(client_address.sin_port) != peer_port) {
                info(verbose, "Received hello reply from unexpected address");
                error_msg(mes_to_receive);
                continue;
            }
            if (mes_to_receive->message != MES_HELLO_REPLY) {
                info(verbose, "Expected HELLO_REPLY but got message number %d", mes_to_receive->message);
                error_msg(mes_to_receive);
                continue;
            }
            if (received_length != (ssize_t)(sizeof(data) + sizeof(node_info) * ntohs(mes_to_receive->count))) {
                info(verbose, "incomplete receiving");
                continue;
            }
            break;
        }
        info(verbose, "Received hello reply from %s:%d", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        for (int i = 0; i < ntohs(mes_to_receive->count); i++) {
            node_info new_node;
            new_node.peer_address_length = mes_to_receive->nodes[i].peer_address_length;
            memcpy(new_node.peer_address, mes_to_receive->nodes[i].peer_address, new_node.peer_address_length);
            new_node.peer_port = ntohs(mes_to_receive->nodes[i].peer_port);
            
            struct sockaddr_in new_node_address = get_server_address(inet_ntoa(*(struct in_addr*)new_node.peer_address), new_node.peer_port);
            mes_to_send = malloc(sizeof(data));
            if (!mes_to_send) {
                syserr("malloc");
            }
            mes_to_send->message = MES_CONNECT;
            int send_flags = 0;
            ssize_t sent_length = sendto(socket_fd, mes_to_send, sizeof(data), send_flags,
                                        (struct sockaddr *) &new_node_address, sizeof(new_node_address));
            if (sent_length < 0) {
                free(mes_to_send);
                syserr("sendto");
            }
            else if (sent_length != sizeof(data)) {
                free(mes_to_send);
                fatal("incomplete sending");
            }
            info(verbose, "Sent Connect to %s:%d", inet_ntoa(new_node_address.sin_addr), ntohs(new_node_address.sin_port));
            free(mes_to_send);
        }

        while (known_nodes_count < ntohs(mes_to_receive->count)) {
            data* connect_ack_mes = malloc(sizeof(data));
            if (!connect_ack_mes) {
                syserr("malloc");
            }
            int flags = 0;
            struct sockaddr_in new_node_address;
            socklen_t address_length = (socklen_t) sizeof(new_node_address);
            ssize_t received_length = recvfrom(socket_fd, connect_ack_mes, sizeof(data), flags,
                                        (struct sockaddr *) &new_node_address, &address_length);
            if (received_length < 0 && (errno == EAGAIN  || errno == EWOULDBLOCK)) {
                info(verbose, "Not all nodes sent back CONNECT_ACK in time");
                free(connect_ack_mes);
                break;
            }
            if (received_length != sizeof(data)) {
                info(verbose, "Receive error");
                free(connect_ack_mes);
                continue;
            }
            if (connect_ack_mes->message != MES_ACK_CONNECT) {
                info(verbose, "Expected CONNECT_ACK but got %d", connect_ack_mes->message);
                free(connect_ack_mes);
                continue;
            }
            bool is_address_ok = false;
            info(verbose, "Received CONNECT_ACK from %s:%d", inet_ntoa(new_node_address.sin_addr), ntohs(new_node_address.sin_port));
            for (int i = 0; i < ntohs(mes_to_receive->count); i++) {
                if (memcmp(&new_node_address.sin_addr, &mes_to_receive->nodes[i].peer_address, sizeof(new_node_address.sin_addr)) == 0 &&
                    ntohs(new_node_address.sin_port) == ntohs(mes_to_receive->nodes[i].peer_port)) {
                    is_address_ok = true;
                    break;
                }
            }
            if (!is_address_ok) {
                info(verbose, "Received CONNECT_ACK from unexpected address");
                free(connect_ack_mes);
                continue;
            }

            node_info new_node;
            new_node.peer_address_length = sizeof(new_node_address.sin_addr);
            memcpy(new_node.peer_address, &new_node_address.sin_addr, new_node.peer_address_length);
            new_node.peer_port = ntohs(new_node_address.sin_port);
            if (known_nodes_count >= MAX_NODES) {
                info(verbose, "Maximum number of nodes reached");
                free(connect_ack_mes);
                break;
            }
            else {
                known_nodes[known_nodes_count++] = new_node;
            }
            info(verbose, "Added node %s:%d", inet_ntoa(*(struct in_addr*)new_node.peer_address), new_node.peer_port);
        }
        free(mes_to_receive);

        node_info new_node;
        new_node.peer_address_length = sizeof(peer_address_in.sin_addr);
        memcpy(new_node.peer_address, &peer_address_in.sin_addr, new_node.peer_address_length);
        new_node.peer_port = ntohs(peer_address_in.sin_port);
        if (known_nodes_count >= MAX_NODES) {
            info(verbose, "Maximum number of nodes reached");
        }
        else {
            known_nodes[known_nodes_count++] = new_node;
        }
        info(verbose, "Added node %s:%d", inet_ntoa(*(struct in_addr*)new_node.peer_address), new_node.peer_port);
    }

    uint8_t sync_lvl = MAX_SYNC_LVL;
    int64_t offset = 0;
    uint64_t send_sync_start_time = current_time_millis(start_time);
    node_info sync_node;
    sync_node.peer_port = 0;
    sync_info sync_process;
    sync_process.waiting_for_message = MES_SYNC_START;
    uint64_t last_sync_time = 0;

    while (true)
    {
        if (current_time_millis(start_time) - last_sync_time > RESET_SYNC_INTERVAL && sync_lvl != 0) {
            sync_lvl = MAX_SYNC_LVL;
            offset = 0;
            sync_node.peer_port = 0;
            info(verbose, "Resetting sync level");
        }
        data* mes_to_receive = malloc(sizeof(data));
        if (!mes_to_receive) {
            syserr("malloc");
        }
        int flags = 0;
        struct sockaddr_in client_address;
        socklen_t address_length = (socklen_t) sizeof(client_address);
        ssize_t received_length = recvfrom(socket_fd, mes_to_receive, sizeof(data), flags,
                                    (struct sockaddr *) &client_address, &address_length);
        bool timeout = false;
        if ((received_length < 0 && (errno == EAGAIN  || errno == EWOULDBLOCK))) {
            timeout = true;
            info(verbose, "Did not receive any message in time");
        }
        if (current_time_millis(start_time) - send_sync_start_time > SYNC_START_INTERVAL) {
            send_sync_start(verbose, socket_fd, known_nodes, &known_nodes_count, offset, start_time, sync_lvl);
            send_sync_start_time = current_time_millis(start_time);
        }
        if (received_length != sizeof(data)) {
            if (!timeout) {
                info(verbose, "Receive error");
            }
            free(mes_to_receive);
            continue;
        }
        switch (mes_to_receive->message) {
        case MES_HELLO:
            handle_hello(verbose, socket_fd, client_address, known_nodes, &known_nodes_count);
            break;
        case MES_CONNECT:
            handle_connect(verbose, socket_fd, client_address, known_nodes, &known_nodes_count);
            break;
        case MES_DELAY_REQUEST:
            handle_delay_request(verbose, socket_fd, client_address, offset, sync_lvl, start_time);
            break;
        case MES_DELAY_RESPONSE:
            handle_delay_response(verbose, client_address, *mes_to_receive, &sync_lvl, &sync_node, &sync_process, start_time, &offset, &last_sync_time);
            break;
        case MES_SYNC_START:
            handle_sync_start(verbose, socket_fd, client_address, *mes_to_receive, known_nodes, &known_nodes_count, &sync_lvl, &sync_node,
                                &sync_process, start_time, &last_sync_time);
            break;
        case MES_LEADER:
            if (mes_to_receive->synchronized == 0) {
                sync_lvl = 0;
                offset = 0;
                send_sync_start_time = 0;
                info(verbose, "Start being a leader ");
            } else if (mes_to_receive->synchronized == MAX_SYNC_LVL && sync_lvl == 0) {
                sync_lvl = MAX_SYNC_LVL;
                info(verbose, "Stop being a leader");
            }
            else {
                info(verbose, "Received non valid leader message");
                error_msg(mes_to_receive);
            }
            break;
        case MES_GET_TIME:
            handle_get_time(verbose, socket_fd, client_address, offset, sync_lvl, start_time);
            break;
        default:
            info(verbose, "Unexpected message type %d from %s:%d", mes_to_receive->message, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            error_msg(mes_to_receive);
            break;
        }
        free(mes_to_receive);
    }
}