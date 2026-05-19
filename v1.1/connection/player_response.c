#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "http_response.h"
#include "../game/game_state.h"
#include "../game/player.h"

void send_player_list(int client_fd, const PlayerList *player_list)
{
    char *json = malloc(256);
    snprintf(json, 256, "{ \"players\": [\"Player1\", \"Player2\"] }");
    send_response(client_fd, "application/json", json);
    free(json);
}





