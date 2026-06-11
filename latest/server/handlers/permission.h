#ifndef PERMISSION_H
#define PERMISSION_H
#include "../main.h"
#include <stdbool.h>

#include "../constants.h"
#include "../../database/database.h"
#include <sodium.h>

typedef struct 
{
    int64_t user_id;
    char database_token[HASH_SIZE];
    int is_current;
} TokenInformation;

int handle_permission_request(int client_fd, const char * request);
DbQueryStatus db_get_token_information(PGconn * conn, TokenInformation * token_information);

#endif