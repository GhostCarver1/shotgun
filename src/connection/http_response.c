#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "http_response.h"

void send_response(int client_fd, const char *content_type, const char *body)
{
    char header[512];

    int body_len = strlen(body);

    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n",
        content_type,
        body_len
    );

    write(client_fd, header, strlen(header));
    write(client_fd, body, body_len);
}

