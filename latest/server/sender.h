#ifndef SENDER_H
#define SENDER_H

#define EMAIL_SIZE 128
#define PASSWORD_SIZE 128
#define USER_NAME_SIZE 32
#define ID_SIZE 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sodium.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include "constants.h"
#include "../database/database.h"
#include "helpers/json_helper.h"

#include "handlers/authentication/permission.h"
#include "handlers/authentication/signup.h"
#include "handlers/authentication/login.h"
#include "handlers/games/post_game.h"

void send_response(int client, const char *content_type, const char *body);
void send_failure(int client_fd, int status_code, const char * reason);
void send_file(int client, const char *filename);
void send_css(int client, const char *filename);
void setup_server_socket(int *server_fd, struct sockaddr_in * address);
void send_response(int client_fd, const char *content_type, const char *body);
int setup_webpage();
void * handle_response(void * arg);

#endif