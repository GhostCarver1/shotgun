#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "http_response.h"
#include "../game/game_state.h"
#include "../game/player.h"

void send_game_state(int client_fd, const GameState *game_state)
{
    char *json = get_game_state_json(game_state);
    send_response(client_fd, "application/json", json);
    free(json);
}




