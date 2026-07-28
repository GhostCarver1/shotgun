
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
    snprintf(response, sizeof(response), "{\"status\":\"success\",\"game_id\":\"%s\",\"player_count\":%d, \"player_ids\":[", qgresponse.game_id, qgresponse.player_count);
    int used = strlen(response);
    for (int i = 0; i < qgresponse.player_count; i++)
    {
        snprintf(response+used, sizeof(response)-used,"\"%s\", ", qgresponse.player_ids[i]);
        used+=strlen(response+used);
    }
    snprintf(response+used-2, sizeof(response)-used+2," ]}");
    send_response(client_fd, "application/json", response);
}

Result db_query_game(PGconn * conn, const char game_id[ID_SIZE], QGResponse * qgresponse)
{

    char *sql1 = "Select * from games where game_id = $1 and active = TRUE;";

    const char *params1[1] = {game_id};

    PGresult *res1 = PQexecParams(
        conn,sql1,1,NULL,params1,NULL,NULL,0
    );

    if (PQresultStatus(res1) != PGRES_TUPLES_OK) {
        PQclear(res1);
        return create_error(ERROR_TYPE_DATABASE, ERROR_CODE_DATABASE_QUERY_INVALID, "Query invalid for quering game: %s\n", PQerrorMessage(conn));
    }

    if (PQntuples(res1) == 0) {
        PQclear(res1);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"Unable to find game in database: %s\n", game_id);
    }

    strncpy(qgresponse->game_id, PQgetvalue(res1,0,0),ID_SIZE - 1);
    qgresponse->game_id[ID_SIZE - 1] = '\0';

    PQclear(res1);

    char *sql2 = "select * from game_players where game_id = $1";

    const char *params2[1] = {game_id};

    PGresult *res2 = PQexecParams(
        conn,sql2,1,NULL,params2,NULL,NULL,0
    );

    if (PQresultStatus(res2) != PGRES_TUPLES_OK) {
        PQclear(res2);
        return create_error(ERROR_TYPE_DATABASE, ERROR_CODE_DATABASE_QUERY_INVALID, "Query invalid for quering game: %s\n", PQerrorMessage(conn));
    }

    if (PQntuples(res2) == 0) {
        PQclear(res2);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"There were no players attached to the game: %s\n", game_id);
    }

    if (PQntuples(res2) >= MAX_PLAYERS)
    {
        PQclear(res2);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID,"THERE WERE TO MANY PLAYERS TO FIT IN MAX PLAYER SIZE: %s\n", game_id);
    }

    int player_count = PQntuples(res2);
    for (int i = 0; i < player_count ; i++)
    {
        strncpy(qgresponse->player_ids[i], PQgetvalue(res1,i,1),ID_SIZE - 1);
        qgresponse->player_ids[i][ID_SIZE - 1] = '\0';
    }
    qgresponse->player_count = player_count;

    PQclear(res2);

    return create_success();
}