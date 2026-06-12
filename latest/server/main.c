#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sodium.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include "main.h"
#include "constants.h"

#include "../database/database.h"

#include "helpers/json_helper.h"

#include "handlers/login.h"
#include "handlers/permission.h"
#include "handlers/signup.h"


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

int setup_webpage()
{

    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    setup_server_socket(&server_fd,&address);

    while (1) {

        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        pthread_t thread_id;
        int *client_ptr = malloc(sizeof(int));
        if (client_ptr == NULL) {
            perror("malloc");
            close(client_fd);
            continue;
        }

        *client_ptr = client_fd;

        if (pthread_create(&thread_id, NULL, handle_response, client_ptr) != 0) {
            perror("pthread_create");
            close(client_fd);
            free(client_ptr);
            continue;
        }

        pthread_detach(thread_id);
    }

    return 0;
}

void * handle_response(void * arg)
{
    int client_fd = *(int *)arg;
    free(arg);

    char buffer[4096];

    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    buffer[bytes_read] = '\0';

    printf("Bytes read: %zd\n", bytes_read);
    printf("Raw request data:\n%s\n", buffer);

    if (bytes_read <= 0) {
        if (bytes_read < 0) {
            perror("read");
        }
        close(client_fd);
        return NULL;
    }

    printf("Request: \n%s\n", buffer);



    if (strncmp(buffer, "GET / ", 6) == 0) {
        printf("Serving index.html\n");
        send_file(client_fd, "web/index.html");
    }
    else if (strncmp(buffer, "GET /style.css", 14) == 0) {
        send_css(client_fd, "web/style.css");
    }
    else if (strncmp(buffer, "GET /", 5) == 0) 
    {
        char file_path[MAX_FILE_PATH];
        char method[15];
        snprintf(file_path, sizeof(file_path), "web/");
        sscanf(buffer, "%15s %255s", method, file_path+3);
        
        printf("serving file: %s \n", file_path);

        send_file(client_fd, file_path);
    }
    else if (strncmp(buffer, "POST /login ", 12) == 0) {
        printf("PROCESSING LOGIN REQUEST\n");
        handle_login_request(client_fd, buffer);
    }
    else if (strncmp(buffer, "POST /permission ", 17) == 0)
    {
        printf("PROCESSING PERMISSION REQUEST\n");
        handle_permission_request(client_fd, buffer);
    }
    else if (strncmp(buffer, "POST /signup ", 13) == 0)
    {
        printf("PROCESSING SIGNUP REQUEST\n");
        handle_signup_request(client_fd, buffer);
    }
    else {
        printf("Unknown request, sending 404\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\",\"reason\":\"unknown request\"}");
    }

    close(client_fd);


    return NULL;
}


int main()
{
    setup_webpage();
}

