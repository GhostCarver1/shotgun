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
#include "helpers/json_helper.h"
#include <pthread.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "handlers/permission.h"



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
    else if (strncmp(buffer, "POST /login ", 12) == 0) {
        printf("PROCESSING LOGIN REQUEST\n");
        handle_login_request(client_fd, buffer);
    }
    else if (strncmp(buffer, "POST /permission ", 17) == 0)
    {
        printf("PROCESSING PERMISSION REQUEST\n");
        handle_permission_request(client_fd, buffer);
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