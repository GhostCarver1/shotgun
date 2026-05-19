#ifndef GAME_STATE_RESPONSE_H
#define GAME_STATE_RESPONSE_H

#include "../game/game_state.h"

typedef struct {
    GameState * game_state;
} GameStateResponse;

void send_game_state(int client_fd, const GameState *game_state);

#endif