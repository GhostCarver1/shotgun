#include <sodium.h>
#include "../../database/database.h"
#include "login.h"
#include "../main.h"
#include <stdio.h>
#include <string.h>
#include "login.h"




int handle_login_request(int client_fd, const char * request)
{

    char *body = strstr(request, "\r\n\r\n");
    if (!body) {
        printf("No Body Found in Login Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"no body found\"}");
        return 0;
    }
    body += 4;

    UserAccount user_account;

    int email_ok = extract_json_value(body, "email", user_account.email, sizeof(user_account.email));
    int password_ok = extract_json_value(body, "password", user_account.password, sizeof(user_account.password));

    if (!email_ok)
    {
        printf("Could not extract email feild json.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"could not extract email\"}");
        return 0;
    }
    if (!password_ok)
    {
        printf("Could not extract password feild of json.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"could not extract password\"}");
        return 0;
    }

    if (sodium_init() < 0)
    {
        printf("libsodium init failed\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"libsodium init failed\"}");
        return 0;
    }
    

    PGconn *conn = db_connect();
    
    int database_status = db_load_player_info(conn, &user_account);

    db_disconnect(conn);

    if (database_status==DB_EMPTY)
    {
        printf("Database returned a empty list\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"user email not found\"}");
        return 0;
    }
    if (database_status==DB_QUERY_INVALID)
    {
        printf("There was a syntax error in the query");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"query syntax error\"}");
        return 0;
    }
    if (user_account.hash[0] == '\0')
    {
        printf("User not found in database.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"user not found\"}");
        return 0;
    }

    printf("HASH FROM DB: %s\n", user_account.hash);

    if (crypto_pwhash_str_verify(user_account.hash, user_account.password, strlen(user_account.password)) != 0)
    {
        printf("Password verification failed.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"password verification failed\"}");
        return 0;
    }
    
    printf("{\n");
    printf("    EMAIL: %s\n", user_account.email);
    printf("    PASSWORD: %s\n", user_account.password);
    printf("    HASH: %s\n",user_account. hash);
    printf("    USER_ID: %s\n", user_account.id);
    printf("    USER_NAME: %s\n", user_account.user_name);
    printf("}\n");

    char response[256];

    snprintf(response, sizeof(response), "{\"status\":\"success\", \"user_id\":\"%s\", \"user_name\":\"%s\"}", user_account.id, user_account.user_name);
    send_response(client_fd, "application/json", response);

}

DbQueryStatus db_load_player_info(PGconn * conn, UserAccount * user_account)
{
    const char *sql = 
        "SELECT user_hash, player_id, user_name from players where user_email = ($1);";
    
    const char *params[1] = {
        user_account->email
    };

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "get player info failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return DB_QUERY_INVALID;
    }

    if (PQntuples(res) == 0) {
        fprintf(stderr, "No player found with email: %s\n", user_account->email);
        PQclear(res);
        return DB_EMPTY;
    }

    strncpy(user_account->hash, PQgetvalue(res,0,0),crypto_pwhash_STRBYTES - 1);
    strncpy(user_account->id, PQgetvalue(res,0,1), ID_SIZE - 1);
    strncpy(user_account->user_name, PQgetvalue(res,0,2), USER_NAME_SIZE - 1);
    user_account->hash[crypto_pwhash_STRBYTES - 1] = '\0';
    user_account->id[ID_SIZE - 1] = '\0';
    user_account->user_name[USER_NAME_SIZE - 1] = '\0';

    PQclear(res);
    return DB_SUCCESS;
}