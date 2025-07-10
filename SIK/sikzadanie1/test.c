#include "message_handler.h"
#include "err.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>

int main() {
    char bind_address[9] = "127.0.0.1";
    int port1 = 8080;
    int port2 = 8081;
    int port3 = 8082;
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        syserr("cannot create a socket");
    }
    struct sockaddr_in my_address_in = get_server_address(NULL, 0);
    if (bind(socket_fd, (struct sockaddr *) &my_address_in, (socklen_t) sizeof(my_address_in)) < 0) {
        syserr("bind");
    }
    struct sockaddr_in client_address2 = get_server_address(bind_address, port2);
    struct sockaddr_in client_address = get_server_address(bind_address, port1);
    struct sockaddr_in client_address3 = get_server_address(bind_address, port3);
    data *mes_to_send = malloc(sizeof(data));
    if (!mes_to_send) {
        syserr("malloc");
    }
    mes_to_send->message = MES_LEADER;
    sendto(socket_fd, mes_to_send, sizeof(data), 0,
           (struct sockaddr *) &client_address, sizeof(client_address));
    
    sleep(10);

    mes_to_send->message = MES_GET_TIME;
    sendto(socket_fd, mes_to_send, sizeof(data), 0,
           (struct sockaddr *) &client_address, sizeof(client_address));
    sendto(socket_fd, mes_to_send, sizeof(data), 0,
            (struct sockaddr *) &client_address2, sizeof(client_address2));
    sendto(socket_fd, mes_to_send, sizeof(data), 0,
            (struct sockaddr *) &client_address3, sizeof(client_address3));
    
    data mes_to_receive;
    int flags = 0;
    socklen_t address_length = (socklen_t) sizeof(client_address);
    ssize_t received_length = recvfrom(socket_fd, &mes_to_receive, sizeof(data), flags,
                                       (struct sockaddr *) &client_address, &address_length);
    printf("Received message type %d from %s:%d\n", mes_to_receive.message, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    printf("Synchronized: %d\n", mes_to_receive.synchronized);
    printf("Timestamp: %" PRIu64 "\n", ntohll(mes_to_receive.timestamp));

    flags = 0;
    address_length = (socklen_t) sizeof(client_address2);
    received_length = recvfrom(socket_fd, &mes_to_receive, sizeof(data), flags,
                                       (struct sockaddr *) &client_address2, &address_length);
    printf("Received message type %d from %s:%d\n", mes_to_receive.message, inet_ntoa(client_address2.sin_addr), ntohs(client_address2.sin_port));
    printf("Synchronized: %d\n", mes_to_receive.synchronized);
    printf("Timestamp: %" PRIu64 "\n", ntohll(mes_to_receive.timestamp));

    flags = 0;
    address_length = (socklen_t) sizeof(client_address3);
    received_length = recvfrom(socket_fd, &mes_to_receive, sizeof(data), flags,
                                       (struct sockaddr *) &client_address3, &address_length);
    printf("Received message type %d from %s:%d\n", mes_to_receive.message, inet_ntoa(client_address3.sin_addr), ntohs(client_address3.sin_port));
    printf("Synchronized: %d\n", mes_to_receive.synchronized);
    printf("Timestamp: %" PRIu64 "\n", ntohll(mes_to_receive.timestamp));

}