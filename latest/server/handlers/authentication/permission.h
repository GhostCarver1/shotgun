#ifndef PERMISSION_H
#define PERMISSION_H

#include "../../constants.h"
#include "../../../database/database.h"
#include "../../result/result.h"
#include "../../helpers/json_helper.h"
#include "login.h"
#include "../../main.h"

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sodium.h>
typedef struct
{
    char token[HASH_SIZE];
    char user_id[BIGSERIAL_STRING_LENGTH];
} PermissionRequest;

typedef struct
{
    char user_token_hash[TOKEN_HEX_LEN];
    char db_token_hash[crypto_generichash_BYTES * 2 + 1];
} PermissionContext;

typedef struct 
{
    int permission;
} PermissionResponse;

int handle_permission_request(int client_fd, const char * request);
Result db_get_token_information(PGconn * conn, PermissionRequest * permission_request, PermissionContext * permission_context, PermissionResponse * permission_response);

#endif