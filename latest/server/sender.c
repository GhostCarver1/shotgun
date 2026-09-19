#include "sender.h"

void send_failure(int client_fd, int status_code, const char * reason)
{
    char header[512];

    char body[MAX_FAILURE_RESPONSE_SIZE];

    snprintf(body, MAX_FAILURE_RESPONSE_SIZE,"{\"status\":\"failure\", \"reason\":\"%s\"\n}", reason);

    snprintf(header, sizeof(header),
        "HTTP/1.1 %d OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %lu\r\n"
        "Cache-Control: no-cache, no-store, must-revalidate\r\n"
        "Pragma: no-cache\r\n"
        "Expires: 0\r\n"
        "Connection: close\r\n"
        "\r\n",
        status_code,
        strlen(body)
    );

    write(client_fd, header, strlen(header));
    write(client_fd, body, strlen(body));
    
}

void send_response(int client_fd, const char *content_type, const char *body)
{
    char header[512];

    int body_len = strlen(body);

    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Cache-Control: no-cache, no-store, must-revalidate\r\n"
        "Pragma: no-cache\r\n"
        "Expires: 0\r\n"
        "Connection: close\r\n"
        "\r\n",
        content_type,
        body_len
    );

    write(client_fd, header, strlen(header));
    write(client_fd, body, body_len);
}

void send_file(int client, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        char error[512];
        snprintf(error, sizeof(error), "File not found :%s\r\n", filename);
        send_response(client, "text/plain", error);
        return;
    }

    char buffer[4096];
    size_t bytes;

    send(client,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n",
        44, 0);

    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client, buffer, bytes, 0);
    }

    fclose(file);
}

void send_css(int client, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        char error[512];
        snprintf(error, sizeof(error), "File not found :%s\r\n", filename);
        send_response(client, "text/plain", error);
        return;
    }

    char buffer[4096];
    size_t bytes;

    send(client,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/css\r\n"
        "\r\n",
        44, 0);

    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client, buffer, bytes, 0);
    }

    fclose(file);
}

void setup_server_socket(int *server_fd, struct sockaddr_in * address)
{

    // CODE SOURCED FROM GEEKS FOR GEEKS ORG

    int new_socket;
    ssize_t valueread;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(8080);

     if (bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(*server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port 8080...\n");
}





