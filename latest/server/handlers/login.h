#ifndef LOGIN_H
#define LOGIN_H

#include "../constants.h"
#include "../../database/database.h"

#include "../result/result.h"

#include <sodium.h>

typedef struct
{
    char email[EMAIL_SIZE];
    char password[PASSWORD_SIZE];
} LoginRequest;

typedef struct
{
    char db_password_hash[crypto_pwhash_STRBYTES];
    char db_token_hash[crypto_generichash_BYTES * 2 + 1];
    char user_name[USER_NAME_SIZE];
    char id[ID_SIZE];
} LoginContext;

typedef struct
{
    char token[TOKEN_HEX_LEN];
} LoginResponse;

int handle_login_request(int client_fd, const char * request);

Result db_gather_login_context(PGconn * conn, const LoginRequest * login_request, LoginContext * login_context);
Result db_store_hashed_token(PGconn * conn, const char * id, const char * token_hash);

int hash_token(const char *token_hex, char hash_hex[crypto_generichash_BYTES * 2 + 1]);
int generate_token(char token_hex[TOKEN_HEX_LEN]);

#endif