#ifndef DELETE_GAME_H
#define DELETE_GAME_H

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
    char game_id[ID_SIZE];
    char token[TOKEN_HEX_LEN];
} DGRequest;

typedef struct
{
    
} DGContext;

typedef struct
{

} DGResponse;

int handle_delete_game_request(int client_fd, const char * request);
Result db_set_game_inactive(PGconn * conn, const char game_id[ID_SIZE]);

#endif