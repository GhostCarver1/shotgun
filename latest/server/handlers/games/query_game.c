
#include "query_game.h"

#define STRINGIFY_INNER(x) #x
#define STRINGIFY(x) STRINGIFY_INNER(x)


int handle_query_game_request(int client_fd, const char * request)
{

    char *body = strstr(request, "\r\n\r\n");
    
    if (!body) {
        printf("No Body Found in Login Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"no body found\"}");
        return 0;
    }
    body += 4;

    QGRequest qgrequest;
    QGContext qgcontext;
    QGResponse qgresponse;

    JsonFeild json_feilds[] = {
        FIELD(token, qgrequest),
        FIELD(game_id, qgrequest)
    };

    Result extracting_json_feilds_result = extract_json_feilds(body, json_feilds, 2);

    if (extracting_json_feilds_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_json_feilds_result.message);
        return 0;
    }

    PGconn * conn = db_connect();
    Result db_get_game = db_query_game(conn, qgrequest.game_id, &qgresponse);
    db_disconnect(conn);
    
    if (db_get_game.status != SUCCESS)
    {
        send_failure(client_fd, 400, db_get_game.message);
        return 0;
    } 

    char response[MAX_RESPONSE_SIZE];
    snprintf(response, sizeof(response), "{\"status\":\"success\",\"game_id\":\"%s\"}", qgresponse.game_id);
    send_response(client_fd, "application/json", response);
}

Result db_query_game(PGconn * conn, const char game_id[ID_SIZE], QGResponse * qgresponse)
{

    const char *sql = "Select * from games where game_id = $1;";

    const char *params[1] = {game_id};

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return create_error(ERROR_TYPE_DATABASE, ERROR_CODE_DATABASE_QUERY_INVALID, "Query invalid for quering game: %s\n", PQerrorMessage(conn));
    }

    if (PQntuples(res) == 0) {
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"Unable to reserve game into database: %s\n", game_id);
    }

    strncpy(qgresponse->game_id, PQgetvalue(res,0,0),ID_SIZE - 1);
    qgresponse->game_id[ID_SIZE - 1] = '\0';

    PQclear(res);

    return create_success();
}