#ifndef LOGIN_H
#define LOGIN_H
#include "../main.h"

typedef struct 
{
    char email[EMAIL_SIZE];
    char password[PASSWORD_SIZE];
    char hash[crypto_pwhash_STRBYTES];
    char id[ID_SIZE];
    char user_name[USER_NAME_SIZE];
} UserAccount;

int handle_login_request(int client_fd, const char * request);
int db_load_player_info(PGconn * conn, UserAccount * user_account);

#endif