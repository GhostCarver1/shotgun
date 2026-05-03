#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../game/game_state.h"
#include "http_response.h"
#include "server.h"
#include "routes.h"




void send_file(int client, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        send_response(client, "text/plain", "File not found");
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


int setup_webpage(GameState * game_state)
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

        printf("Request:\n%s\n", buffer);

        
        handle_request(client_fd, buffer, game_state);

    }
    close(client_fd);
    return 0;
}