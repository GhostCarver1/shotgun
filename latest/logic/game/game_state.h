#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../logic_constants.h"
#include "shotgun.h"

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sodium.h>

typedef struct {
    Shotgun shotgun;
    size_t player_count;
    char current_player_id[ID_SIZE];
    char game_id[ID_SIZE];
    char player_ids[MAX_PLAYERS][ID_SIZE];
} GameState;

bool game_state_init(GameState *game, const char *creator_id);




#endif