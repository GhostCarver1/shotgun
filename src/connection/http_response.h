#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

void send_response(int client_fd, const char *content_type, const char *body);

#endif