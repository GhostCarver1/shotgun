#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sodium.h>
#include "../database/database.h"
#include "main.h"
#include "handlers/login.h"



#define HASH_SIZE crypto_pwhash_STRBYTES

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

int extract_json_value(const char *json, const char *key,
                       char *output, size_t output_size)
{
    char pattern[64];

    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);

    char *start = strstr(json, pattern);

    if (!start)
    {
        fprintf(stderr, "Missing JSON key: %s\n", key);
        return 0;
    }

    start += strlen(pattern);

    char *end = strchr(start, '"');

    if (!end)
    {
        fprintf(stderr, "Malformed JSON for key: %s\n", key);
        return 0;
    }

    size_t value_length = end - start;

    if (value_length >= output_size)
    {
        value_length = output_size - 1;
    }

    strncpy(output, start, value_length);
    output[value_length] = '\0';

    return 1;
}

void setup_server_socket(int *server_fd, struct sockaddr_in * address)
{
    * server_fd = socket(AF_INET, SOCK_STREAM,0);
    if (*server_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(8080);

    int opt = 1;
    setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

     if (bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(*server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port 8080...\n");
}


int setup_webpage()
{

    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[4096];

    setup_server_socket(&server_fd,&address);
    

    while (1) {

        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, sizeof(buffer) - 1);

        //printf("Request: \n%s\n", buffer);


        if (strncmp(buffer, "GET / ", 6) == 0) {
            printf("Serving index.html\n");
            send_file(client_fd, "web/index.html");
        }
        else if (strncmp(buffer, "POST /login ", 12) == 0) {
            printf("PROCESSING LOGIN REQUEST\n");
            handle_login_request(client_fd, buffer);
        }
        else {
            printf("Unknown request, sending 404\n");
            send_response(client_fd, "text/plain", "404 Not Found\n");
        }

        close(client_fd);

    }

    return 0;
}

int main()
{
    setup_webpage();
}



int handle_signup_request(int client_fd, const char * request)
{

    char *body = strstr(request, "\r\n\r\n");
    if (!body) {
        printf("No Body Found in Login Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\"}");
        return 0;
    }
    body += 4;

    char email[EMAIL_SIZE] = {0};
    char password[PASSWORD_SIZE] = {0};

    int email_ok = extract_json_value(body, "email", email, sizeof(email));
    int password_ok = extract_json_value(body, "password", password, sizeof(password));

}