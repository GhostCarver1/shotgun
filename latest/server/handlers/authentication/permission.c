#include "permission.h"


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
    
    PermissionRequest permission_request;
    PermissionContext permission_context;
    PermissionResponse permission_response;

    JsonFeild json_feilds[] = {
        FIELD(token, permission_request),
        FIELD(user_id, permission_request)
    };

    Result extracting_json_feilds_result = extract_json_feilds(body, json_feilds, 2);

    if (extracting_json_feilds_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_json_feilds_result.message);
        return 0;
    }

    PGconn *conn = db_connect();
    Result db_get_token_information_status = db_get_token_information(conn, &permission_request, &permission_context, &permission_response);
    db_disconnect(conn);

    if (db_get_token_information_status.status!=SUCCESS)
    {
        printf("getting the token information from the database failed, reason: \"%s\" \n", db_get_token_information_status.message);
        send_failure(client_fd, 400, db_get_token_information_status.message);
        return 0;
    }

    hash_token(permission_request.token, permission_context.user_token_hash);

    if(strncmp(permission_context.db_token_hash, permission_context.user_token_hash, TOKEN_HEX_LEN)!=0)
    {
        printf("The Database Token and inputted token do not match \n");
        send_response(client_fd, "application/json","{\"status\":\"success\",\"permission\":\"denied\",\"reason\":\"The Database Token and hashed token do not match\"}");
        return 0;
    }

    if (permission_response.permission==0)
    {
        printf("The Requested token is expired \n");
        send_response(client_fd, "application/json","{\"status\":\"success\",\"permission\":\"denied\",\"reason\":\"THe requested token is expired\"}");
        return 0;
    }

    printf("The requested token is valid \n");
    send_response(client_fd, "application/json","{\"status\":\"success\",\"permission\":\"allowed\"}");
    return 0;
  
}

Result db_get_token_information(PGconn * conn, PermissionRequest * permission_request, PermissionContext * permission_context, PermissionResponse * permission_response) // * player_id,  char * database_token, size_t database_token_size)
{
    const char *sql = "SELECT TOKEN_HASH, CASE WHEN NOW() BETWEEN token_start AND token_end THEN '1' ELSE '0' END AS is_valid  FROM tokens WHERE player_id = $1;";

    const char *params[1] = {
        permission_request->user_id
    };

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        //fprintf(stderr, "Inserting Token Query Invalid: %s\n", PQerrorMessage(conn));
        return create_error(ERROR_TYPE_DATABASE, ERROR_CODE_DATABASE_QUERY_INVALID, "Getting token informaiton query invalid: \"%s\"\n", PQerrorMessage(conn));
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        //fprintf(stderr, "There was no token for player:  %s\n", permission_request->user_id);
        return create_error(ERROR_TYPE_DATABASE, ERROR_CODE_DATABASE_QUERY_EMPTY, "Getting token informaiton query turned up empty: \"%s\"\n", PQerrorMessage(conn));
    }
    
    printf("PQgetvalue: %s \n",PQgetvalue(res,0,0));
    
    strncpy(permission_context->db_token_hash, PQgetvalue(res,0,0), TOKEN_HEX_LEN - 1);
    permission_context->db_token_hash[TOKEN_HEX_LEN-1]='\0';

    permission_response->permission = atoi(PQgetvalue(res,0,1));
    
    PQclear(res);

    return create_success();
}