#ifndef SIGNUP_H
#define SIGNUP_H
#include <stdbool.h>

#include "../../constants.h"
#include "../../../database/database.h"
#include "../../result/result.h"
#include "../../helpers/json_helper.h"
#include "../../main.h"

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sodium.h>

typedef struct
{
    char email[EMAIL_SIZE];
    char password[PASSWORD_SIZE];
    char user_name[USER_NAME_SIZE];
} SignUpRequest;

typedef struct
{
    char password_hash[crypto_pwhash_STRBYTES];
    char db_token_hash[crypto_generichash_BYTES * 2 + 1];
    char user_name[USER_NAME_SIZE];
    char id[ID_SIZE]; 
} SignUpContext;

typedef struct
{
    
    //char token[TOKEN_HEX_LEN];
} SignUpResponse;

/*
typedef struct 
{
    char received_email[EMAIL_SIZE];
    char received_password[PASSWORD_SIZE];
    char received_user_name[USER_NAME_SIZE];
    char calculated_hashed_password[crypto_pwhash_STRBYTES];
    //char token[TOKEN_HEX_LEN];
} SignUpRequest;
*/

int handle_signup_request(int client_fd, const char * request);
Result db_store_new_user(PGconn * conn, SignUpRequest * signup_request, SignUpContext * signup_context);

#endif