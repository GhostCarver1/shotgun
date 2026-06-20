

#include "signup.h"

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
    SignUpContext signup_context;
    SignUpResponse signup_response;

    JsonFeild json_feilds[] = {
        FIELD(email, signup_request),
        FIELD(password, signup_request),
        FIELD(user_name, signup_request)
    };

    Result extracting_json_feilds_result = extract_json_feilds(body, json_feilds, 2);

    if (extracting_json_feilds_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_json_feilds_result.message);
        return 0;
    }

    if (crypto_pwhash_str(signup_context.password_hash,signup_request.password,strlen(signup_request.password),crypto_pwhash_OPSLIMIT_INTERACTIVE,crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        printf("something whent wrong with the hashings of the password\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"failure hashing the passwordn\"}");
        return 0;
    }

    PGconn *conn = db_connect();
    Result db_store_new_player_result = db_store_new_user(conn, &signup_request, &signup_context);
    db_disconnect(conn);

    if (db_store_new_player_result.status!=SUCCESS)
    {
        printf("storing the player into the database failed, reason: \"%s\" \n", db_store_new_player_result.message);
        send_failure(client_fd, 400, db_store_new_player_result.message);
        return 0;
    }

    send_response(client_fd, "application/json", "{\"status\":\"success\"}");

    return 1;

}

Result db_store_new_user(PGconn * conn, SignUpRequest * signup_request, SignUpContext * signup_context)
{
    const char * sql = 
        "INSERT INTO PLAYERS (user_name, user_email, user_hash) "
        "VALUES ($1, $2, $3) returning player_id;";
    
    const char *params[3] = {
        signup_request->user_name,
        signup_request->email,
        signup_context->password_hash,
    };

    PGresult *res = PQexecParams(
        conn,sql,3,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID,  "Inserting Token Query Invalid: \"%s\"\n", PQerrorMessage(conn));
    }
    if (PQntuples(res) == 0) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,  "Returning the new player id : \"%s\"\n", PQerrorMessage(conn));
    }

    return create_success();

}