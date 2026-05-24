#include <sodium.h>
#include "../../database/database.h"
#include "../main.h"
#include <stdio.h>
#include <string.h>
#include "login.h"
#include "../helpers/json_helper.h"

/*
    POST /login 
    {
        "email":"example@example.com",
        "password":"password"
    }
    
    RESPONSE 
    {
        "status":"success",
        "user_name":"user_name",
        "token_token":"abcedf"
    }

*/

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

    JsonStatus email_ok = extract_json_value(body, "email", user_account.email, sizeof(user_account.email));
    JsonStatus password_ok = extract_json_value(body, "password", user_account.password, sizeof(user_account.password));

    if (email_ok!=JSON_SUCCESS)
    {
        printf("Could not extract email feild json.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"could not extract email\"}");
        return 0;
    }
    if (password_ok!=JSON_SUCCESS)
    {
        printf("Could not extract password field of json.\n");
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
    
    int db_load_player_info_status = db_load_player_info(conn, &user_account);

    

    if (db_load_player_info_status==DB_EMPTY)
    {
        printf("Database returned a empty list\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"user email not found\"}");
        db_disconnect(conn);
        return 0;
    }
    if (db_load_player_info_status==DB_QUERY_INVALID)
    {
        printf("There was a syntax error in the query\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"query syntax error\"}");
        db_disconnect(conn);
        return 0;
    }
    if (user_account.password_hash[0] == '\0')
    {
        printf("User not found in database.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"user not found\"}");
        db_disconnect(conn);
        return 0;
    }

    //printf("HASH FROM DB: %s\n", user_account.password_hash);

    if (crypto_pwhash_str_verify(user_account.password_hash, user_account.password, strlen(user_account.password)) != 0)
    {
        printf("Password verification failed.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"password verification failed\"}");
        db_disconnect(conn);
        return 0;
    }

    generate_token(user_account.token);
    hash_token(user_account.token, user_account.token_hash);


    int db_store_token_status = db_store_hashed_token(conn, &user_account);

    db_disconnect(conn);

    if (db_store_token_status == DB_EMPTY)
    {
        printf("token returned as empty from the database\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"no found token returned from database\"}");
        return 0;
    }
    if (db_store_token_status == DB_QUERY_INVALID)
    {
        printf("There was some syntax error with the database when inserting a token\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"invalid query when inserting a token\"}");
        return 0;
    }


    char response[256];

    snprintf(response, sizeof(response), "{\"status\":\"success\", \"user_id\":\"%s\", \"user_name\":\"%s\", \"token\":\"%s\"}", user_account.id, user_account.user_name, user_account.token);
    send_response(client_fd, "application/json", response);

    return 1;

}

DbQueryStatus db_store_hashed_token(PGconn * conn, UserAccount * user_account)
{
    const char *sql =
        "INSERT INTO tokens (player_id, token_hash, token_start, token_end) "
        "VALUES ($1, $2, NOW(), NOW() + INTERVAL '7 days') "
        "ON CONFLICT (player_id) "
        "DO UPDATE SET "
        "token_hash = EXCLUDED.token_hash, "
        "token_start = NOW(), "
        "token_end = NOW() + INTERVAL '7 days' "
        "RETURNING player_id;";

    const char *params[2] = {
        user_account->id,
        user_account->token_hash
    };

    PGresult *res = PQexecParams(
        conn,sql,2,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Inserting Token Query Invalid: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return DB_QUERY_INVALID;
    }
    if (PQntuples(res) == 0) {
        fprintf(stderr, "Unable to insert token hash into database: %s\n", user_account->email);
        PQclear(res);
        return DB_EMPTY;
    }

    PQclear(res);
    return DB_SUCCESS;

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

    strncpy(user_account->password_hash, PQgetvalue(res,0,0),crypto_pwhash_STRBYTES - 1);
    strncpy(user_account->id, PQgetvalue(res,0,1), ID_SIZE - 1);
    strncpy(user_account->user_name, PQgetvalue(res,0,2), USER_NAME_SIZE - 1);
    user_account->password_hash[crypto_pwhash_STRBYTES - 1] = '\0';
    user_account->id[ID_SIZE - 1] = '\0';
    user_account->user_name[USER_NAME_SIZE - 1] = '\0';

    PQclear(res);
    return DB_SUCCESS;
}

int generate_token(char token_hex[TOKEN_HEX_LEN])
{
    unsigned char token[TOKEN_BYTES];

    randombytes_buf(token, TOKEN_BYTES);
    sodium_bin2hex(token_hex, TOKEN_HEX_LEN, token, TOKEN_BYTES);

    return 1;
}

int hash_token(const char *token_hex, char hash_hex[crypto_generichash_BYTES * 2 + 1])
{
   unsigned char hash[crypto_generichash_BYTES];

    if (token_hex == NULL || hash_hex == NULL) {
        return 0;
    }

    if (crypto_generichash(
            hash,
            sizeof(hash),
            (const unsigned char *)token_hex,
            strlen(token_hex),
            NULL,
            0
        ) != 0) {
        return 0;
    }

    sodium_bin2hex(
        hash_hex,
        crypto_generichash_BYTES * 2 + 1,
        hash,
        sizeof(hash)
    );

    return 1;
}