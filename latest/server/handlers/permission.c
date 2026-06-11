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

/*
    POST /permission
    {
        "token":"abcdef",
        "id":"1"
    }
    
    RESPONSE 
    {
        "status":"success",
        "permission":"allowed",
        "reason":"some reason"
    }

*/

int handle_permission_request(int client_fd, const char * request)
{
    char * body = strstr(request, "\r\n\r\n");
    if (!body) {
        printf("No Body Found in Permission Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\"}");
        return 0;
    }
    body += 4;

    char received_id[BIGSERIAL_STRING_LENGTH]; 
    char received_token[HASH_SIZE];

    JsonStatus token_ok = extract_json_value(body, "token", received_token, HASH_SIZE);
    JsonStatus id_ok = extract_json_value(body, "id", received_id, BIGSERIAL_STRING_LENGTH);



    if (!token_ok)
    {
        printf("Could not extract token field from json.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"Could not extract token from json\"}");
        return 0;
    }
    if (!id_ok)
    {
        printf("Could not extract token field from json.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"Could not extract token from json\"}");
        return 0;
    }


    TokenInformation token_information;


    token_information.user_id = strtoll(received_id, NULL, 10);

    PGconn *conn = db_connect();
    DbQueryStatus query_status = db_get_token_information(conn, &token_information);//id_buffer, database_token, TOKEN_HEX_LEN);
    db_disconnect(conn);


    if (query_status == DB_QUERY_INVALID)
    {
        printf("Bad syntax for the databas equery.\n");
        send_response(client_fd, "application/json","{\"status\":\"success\",\"there was bad syntax with the database query\"}");
        return 0;
    }
    if (query_status == DB_EMPTY)
    {
        printf("This user does not have an associated token. This most likely means that the associated player id is not in the database");
        send_response(client_fd, "application/json","{\"status\":\"failure\",\"reason\":\"could not find a token for the requested player id, player may not be in database\"}");
        return 0;
    }


    char hash_hex[TOKEN_HEX_LEN];
    hash_token(received_token, hash_hex);


    if(strncmp(token_information.database_token, hash_hex, TOKEN_HEX_LEN)!=0)
    {
        printf("The Database Token and inputted token do not match \n");
        send_response(client_fd, "application/json","{\"status\":\"success\",\"permission\":\"denied\",\"reason\":\"The Database Token and hashed token do not match\"}");
        return 0;
    }

    if (token_information.is_current==0)
    {
        printf("The Requested token is expired \n");
        send_response(client_fd, "application/json","{\"status\":\"success\",\"permission\":\"denied\",\"reason\":\"THe requested token is expired\"}");
        return 0;
    }


    char response[256];

    printf("The requested token is valid \n");
    send_response(client_fd, "application/json","{\"status\":\"success\",\"permission\":\"allowed\"}");
    return 0;
    
}

DbQueryStatus db_get_token_information(PGconn * conn, TokenInformation * token_information) // * player_id,  char * database_token, size_t database_token_size)
{
    const char *sql = "SELECT TOKEN_HASH, CASE WHEN NOW() BETWEEN token_start AND token_end THEN '1' ELSE '0' END AS is_valid  FROM tokens WHERE player_id = $1;";

    char user_id_string[BIGSERIAL_STRING_LENGTH];

    snprintf(user_id_string, BIGSERIAL_STRING_LENGTH, "%" PRId64, token_information->user_id);
    user_id_string[BIGSERIAL_STRING_LENGTH-1]='\0';

    const char *params[1] = {
        user_id_string
    };

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Inserting Token Query Invalid: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return DB_QUERY_INVALID;
    }

    if (PQntuples(res) == 0) {
        fprintf(stderr, "There was no token for player:  %s\n", user_id_string);
        PQclear(res);
        return DB_EMPTY;
    }
    
    strncpy(token_information->database_token, PQgetvalue(res,0,0), TOKEN_HEX_LEN);
    token_information->database_token[TOKEN_HEX_LEN-1]='\0';

    token_information->is_current = atoi(PQgetvalue(res,0,1));
    
    PQclear(res);
    return DB_SUCCESS;
}