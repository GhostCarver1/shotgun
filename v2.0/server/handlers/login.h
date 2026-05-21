#ifndef LOGIN_H
#define LOGIN_H
#include "../main.h"

#define TOKEN_BYTES 32
#define TOKEN_HEX_LEN (TOKEN_BYTES * 2 + 1)

typedef struct 
{
    char email[EMAIL_SIZE];
    char password[PASSWORD_SIZE];
    char password_hash[crypto_pwhash_STRBYTES];
    char id[ID_SIZE];
    char user_name[USER_NAME_SIZE];
    char token[TOKEN_HEX_LEN];
    char token_hash[crypto_generichash_BYTES * 2 + 1];
} UserAccount;

int handle_login_request(int client_fd, const char * request);
int db_load_player_info(PGconn * conn, UserAccount * user_account);
int hash_token(const char *token_hex, char hash_hex[crypto_generichash_BYTES * 2 + 1]);
int generate_token(char token_hex[TOKEN_HEX_LEN]);
DbQueryStatus db_store_hashed_token(PGconn * conn, UserAccount * user_account);

#endif