#include "delete_game.h"

int handle_delete_game_request(int client_fd, const char * request)
{

    char *body = strstr(request, "\r\n\r\n");
    if (!body) {
        printf("No Body Found in Login Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"no body found\"}");
        return 0;
    }
    body += 4;

    DGRequest dgrequest;
    DGContext dgcontext;
    DGResponse dgresponse;

    JsonFeild json_feilds[] = {
        FIELD(token, dgrequest),
        FIELD(game_id, dgrequest)
    };

    Result extracting_json_feilds_result = extract_json_feilds(body, json_feilds, 2);

    if (extracting_json_feilds_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_json_feilds_result.message);
        return 0;
    }

    PGconn * conn = db_connect();
    Result db_delete_game = db_set_game_inactive(conn, dgrequest.game_id);
    db_disconnect(conn);
    
    if (db_delete_game.status != SUCCESS)
    {
        send_failure(client_fd, 400, db_delete_game.message);
        return 0;
    }

    char response[MAX_RESPONSE_SIZE];
    snprintf(response, sizeof(response), "{\"status\":\"success\", \"reason\": \"game has successfully been deleted\"}");
    send_response(client_fd, "application/json", response);
}

Result db_set_game_inactive(PGconn * conn, const char game_id[ID_SIZE])
{
    char *sql = "update games set active = false where game_id = $1 returning game_id;";

    const char *params[1] = {game_id};

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE, ERROR_CODE_DATABASE_QUERY_INVALID, "Query invalid for deleteing game: %s", PQerrorMessage(conn));
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"Unable to find game in database to delete: %s", game_id);
    }

    return create_success();

}
