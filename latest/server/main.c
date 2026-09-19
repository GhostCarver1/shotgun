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

#include "handlers/authentication/permission.h"
#include "handlers/authentication/signup.h"
#include "handlers/authentication/login.h"
#include "sender.h"


GameState gamestate;


int main(int argc, char ** argv)
{

    if (sodium_init())
        exit(EXIT_FAILURE);
    setup_webpage();
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
    else if (strncmp(buffer, "POST /games ", 12) == 0)
    {
        printf("PROCESSING POST GAMES\n");
        handle_post_game_request(client_fd,buffer);
    }
    else if (strncmp(buffer, "DELETE /game", 12)==0)
    {
        printf("PROCESSING DELETE GAME REQUEST\n");
        handle_delete_game_request(client_fd, buffer);
    }
    else if (strncmp(buffer, "QUERY /game", 11) == 0) {
        printf("PROCESSING QUERY GAME REQUEST\n");
        handle_query_game_request(client_fd, buffer);
    }
    else {
        printf("Unknown request, sending 404\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\",\"reason\":\"unknown request\"}");
    }

    close(client_fd);


    return NULL;
}


