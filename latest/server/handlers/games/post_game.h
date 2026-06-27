#ifndef POST_GAME_H
#define POST_GAME_H

#include "../../constants.h"
#include "../../../database/database.h"
#include "../../result/result.h"
#include "../../helpers/json_helper.h"
#include "../../sender.h"

#include "../../../logic/game/game_state.h"

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sodium.h>

typedef struct
{
    char token[TOKEN_HEX_LEN];
    char id[ID_SIZE];
    char *player_names[MAX_PLAYERS];
} PGRequest;

typedef struct
{
    
} PGContext;

typedef struct
{

} PGResponse;

int handle_post_game_request(int client_fd, const char * request);

#endif