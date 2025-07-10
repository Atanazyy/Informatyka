#ifndef SERVER_HANDLE_MESSAGES_H
#define SERVER_HANDLE_MESSAGES_H

#include "server_utils.h"
#include "common.h"
#include "err.h"

void handle_message(server_data *data, int player_id, char *message);
void handle_hello(server_data *data, int player_id, char *player_name);
void handle_put(server_data *data, int player_id, char *message);

#endif