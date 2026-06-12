#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define EMAIL_SIZE 128
#define PASSWORD_SIZE 128
#define USER_NAME_SIZE 32
#define ID_SIZE 32

void send_response(int client, const char *content_type, const char *body);
void send_failure(int client_fd, int status_code, const char * reason);
void send_file(int client, const char *filename);
void send_css(int client, const char *filename);
void setup_server_socket(int *server_fd, struct sockaddr_in * address);
void send_response(int client_fd, const char *content_type, const char *body);
int setup_webpage();
int extract_json_value(const char *json, const char *key,
                       char *output, size_t output_size);
int main();
void * handle_response(void * arg);


#endif