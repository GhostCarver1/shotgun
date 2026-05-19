#include <stdio.h>
#include <string.h>

#include "../game/game_state.h"
#include "routes.h"
#include "server.h"
#include "game_state_response.h"
#include "player_response.h"

// handle request is found in routes.c, but we need to declare it here to avoid a circular dependency

void handle_request(int client_fd, const char *buffer, GameState *game_state)
{
    if (strncmp(buffer, "GET / ", 6) == 0) {
        send_file(client_fd, "../web/index.html");
        printf("Sent index.html to client %d\n", client_fd);
    }
    else if (strncmp(buffer, "GET /game_state ", 17) == 0) {
        send_game_state(client_fd, game_state);
    }
    else if (strncmp(buffer, "GET /players ", 13) == 0) {
        send_player_list(client_fd, &game_state->player_list);
        //send_response(client_fd, "text/plain", "Not found");
    }
    else {
        send_response(client_fd, "text/plain", "Not found");
    }
}


/*
const response = await fetch("http://localhost:8080/players", {
            method: "POST"
        });
        */