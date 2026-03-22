#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void send_response(int client, const char *content_type, const char *body);
void send_file(int client, const char *filename);
void setup_server_socket(int *server_fd, struct sockaddr_in * address);
int setup_webpage(GameState * game_state);


#endif