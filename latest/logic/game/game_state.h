#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../logic_constants.h"

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sodium.h>

typedef struct {
    size_t player_count;
    char game_id[ID_SIZE];
    char player_ids[MAX_PLAYERS][ID_SIZE];
} GameState;



#endif