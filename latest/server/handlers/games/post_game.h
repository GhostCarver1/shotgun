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
    char user_id[ID_SIZE];
    char player_ids[MAX_PLAYERS][ID_SIZE];
    
} PGRequest;

typedef struct
{
    size_t players;
} PGContext;

typedef struct
{
    char game_id[ID_SIZE];
} PGResponse;

int handle_post_game_request(int client_fd, const char * request);
Result db_reserve_game_id(PGconn * conn, char game_id[ID_SIZE]);
Result db_connect_game_to_player_ids(PGconn * conn, int player_count, char game_id[ID_SIZE], char player_ids[MAX_PLAYERS][ID_SIZE]);

int get_player_count(char player_ids[MAX_PLAYERS][ID_SIZE]);
#endif