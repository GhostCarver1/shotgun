#include "permission.h"
#include "../main.h"
#include "../helpers/json_helper.h"
#include "../../database/database.h"
#include "login.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../constants.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "login.h"
#include "signup.h"
#include "../result/result.h"

/*
    POST /signup
    {
        "user_email":"abcdef@example.com",
        "user_name":""
        "user_password":""
    }
    
    
    RESPONSE 
    {
        "status":"success",
        "user_id":"int",
        "token_token":"abcedf"
    }

*/



int handle_signup_request(int client_fd, const char * request)
{
    char *body = strstr(request, "\r\n\r\n");
    if (!body) {
        printf("No Body Found in Login Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\"}");
        return 0;
    }
    body += 4;

    SignUpRequest signup_request;

    Result extract_email_result = extract_json_value(body, "email", signup_request.received_email, USER_NAME_SIZE);
    Result extract_user_name_result = extract_json_value(body, "user_name",signup_request.received_user_name, USER_NAME_SIZE);
    Result extract_password_result = extract_json_value(body, "password", signup_request.received_password, PASSWORD_SIZE);

    if (extract_email_result.status == ERROR)
    {
        send_failure(client_fd, 400, extract_email_result.message);
        return 0;
    }
    if (extract_user_name_result.status == ERROR)
    {
        send_failure(client_fd, 400, extract_user_name_result.message);
        return 0;
    }
    if (extract_password_result.status == ERROR)
    {
        send_failure(client_fd, 400, extract_password_result.message);
        return 0;
    }

    if (crypto_pwhash_str(signup_request.calculated_hashed_password,signup_request.received_password,strlen(signup_request.received_password),crypto_pwhash_OPSLIMIT_INTERACTIVE,crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        printf("something whent wrong with the hashings of the password\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"failure hashing the passwordn\"}");
        return 0;
    }

    PGconn *conn = db_connect();
    DbQueryStatus query_status = db_store_new_user(conn, &signup_request);
    db_disconnect(conn);

    if (DB_QUERY_INVALID == query_status)
    {
        printf("there was something wrong with the query\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"failure quering the databse, most likely the user already exists\"}");
        return 0;
    }

    if (DB_EMPTY) {
        printf("Was unable to insert the query into the database\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"player already exists in database\"}");
        return 0;
    }

    send_response(client_fd, "application/json", "{\"status\":\"success\"}");

    return 1;

}

DbQueryStatus db_store_new_user(PGconn * conn, SignUpRequest * signup_request)
{
    const char * sql = 
        "INSERT INTO PLAYERS (user_name, user_email, user_hash) "
        "VALUES ($1, $2, $3) returning player_id;";
    
    const char *params[3] = {
        signup_request->received_user_name,
        signup_request->received_email,
        signup_request->calculated_hashed_password
    };

    PGresult *res = PQexecParams(
        conn,sql,3,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Inserting Token Query Invalid: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return DB_QUERY_INVALID;
    }
    if (PQntuples(res) == 0) {
        fprintf(stderr, "Unable to create new player into database");
        PQclear(res);
        return DB_EMPTY;
    }

}