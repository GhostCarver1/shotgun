#include <sodium.h>
#include "../../database/database.h"
#include "../main.h"
#include <stdio.h>
#include <string.h>
#include "login.h"
#include "../helpers/json_helper.h"
#include "../result/result.h"

int handle_login_request(int client_fd, const char * request)
{

    char *body = strstr(request, "\r\n\r\n");
    
    if (!body) {
        printf("No Body Found in Login Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"no body found\"}");
        return 0;
    }
    body += 4;

    LoginRequest login_request;
    LoginContext login_context;
    LoginResponse login_response;

    Result extract_email_result = extract_json_value(body, "email", login_request.email, sizeof(login_request.email));
    Result extract_password_result = extract_json_value(body, "password", login_request.password, sizeof(login_request.password));

    if (extract_email_result.status == ERROR)
    {
        send_failure(client_fd, 400, extract_email_result.message);
        return 0;
    }

    if (extract_password_result.status == ERROR)
    {
        send_failure(client_fd, 400, extract_password_result.message);
        return 0;
    }

    PGconn *conn = db_connect();
    Result gather_login_context_result = db_gather_login_context(conn, &login_request, &login_context);
    db_disconnect(conn);

    if (gather_login_context_result.status!=SUCCESS)
    {
        printf("gathering login context result failed: %s \n", gather_login_context_result.message);
        send_failure(client_fd, 400, gather_login_context_result.message);
        return 0;
    }

    if (crypto_pwhash_str_verify(login_context.db_password_hash, login_request.password, strlen(login_request.password)) != 0)
    {
        printf("Password verification failed.\n");
        send_failure(client_fd, 401, "INCORRECT PASSWORD");
        return 0;
    }

    generate_token(login_response.token);
    hash_token(login_response.token, login_context.db_token_hash);

    conn = db_connect();
    Result db_store_hashed_token_result = db_store_hashed_token(conn,login_context.id,login_context.db_token_hash);
    db_disconnect(conn);

    if (db_store_hashed_token_result.status!=SUCCESS)
    {
        printf("storing the hashed token in the database failed, reason: %s \n", db_store_hashed_token_result.message);
        send_failure(client_fd, 400, db_store_hashed_token_result.message);
        return 0;
    }

    char response[MAX_RESPONSE_SIZE];

    snprintf(response, sizeof(response), "{\"status\":\"success\", \"user_id\":\"%s\", \"user_name\":\"%s\", \"token\":\"%s\"}", login_context.id, login_context.user_name, login_response.token);
    send_response(client_fd, "application/json", response);

    return 1;

}

Result db_store_hashed_token(PGconn * conn, const char * id, const char * token_hash)
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
        id,
        token_hash
    };

    PGresult *res = PQexecParams(
        conn,sql,2,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID, "Inserting Token Query Invalid: %s\n", PQerrorMessage(conn));
    }
    if (PQntuples(res) == 0) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"Unable to insert token hash into database: %s\n", id);
    }

    PQclear(res);
    return create_success();
}

Result db_gather_login_context(PGconn * conn, const LoginRequest * login_request, LoginContext * login_context)
{
    const char *sql = "SELECT user_hash, player_id, user_name from players where user_email = ($1);";

    const char *params[1] = {login_request->email};

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID,"get player info failed: %s\n", PQerrorMessage(conn));
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"No player found with email: %s ", login_request->email);
    }

    strncpy(login_context->db_password_hash, PQgetvalue(res,0,0),crypto_pwhash_STRBYTES - 1);
    strncpy(login_context->id, PQgetvalue(res,0,1), ID_SIZE - 1);
    strncpy(login_context->user_name, PQgetvalue(res,0,2), USER_NAME_SIZE - 1);
    login_context->db_password_hash[crypto_pwhash_STRBYTES - 1] = '\0';
    login_context->id[ID_SIZE - 1] = '\0';
    login_context->user_name[USER_NAME_SIZE - 1] = '\0';

    return create_success();

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