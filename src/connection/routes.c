#include <stdio.h>
#include <string.h>

#include "../game/game_state.h"
#include "routes.h"
#include "server.h"
#include "game_state_response.h"

void handle_request(int client_fd, const char *buffer, GameState *game_state)
{
    if (strncmp(buffer, "GET / ", 6) == 0) {
        send_file(client_fd, "../web/index.html");
    }
    else if (strncmp(buffer, "POST /game_state ", 17) == 0) {
        send_game_state(client_fd, game_state);
    }
    else {
        //send_response(client_fd, "text/plain", "Not found");
    }
}