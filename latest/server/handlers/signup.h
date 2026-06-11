#ifndef SIGNUP_H
#define SIGNUP_H
#include "../main.h"
#include <stdbool.h>

#include "../constants.h"
#include "../../database/database.h"
#include <sodium.h>

typedef struct 
{
    char received_email[EMAIL_SIZE];
    char received_password[PASSWORD_SIZE];
    char received_user_name[USER_NAME_SIZE];
    char calculated_hashed_password[crypto_pwhash_STRBYTES];
    //char token[TOKEN_HEX_LEN];
} SignUpRequest;

int handle_signup_request(int client_fd, const char * request);
DbQueryStatus db_store_new_user(PGconn * conn, SignUpRequest * signup_request);

#endif