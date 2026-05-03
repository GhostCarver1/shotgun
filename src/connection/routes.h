#ifndef ROUTES_H
#define ROUTES_H

#include "../game/game_state.h"

void handle_request(int client_fd, const char *buffer, GameState *game_state);

#endif