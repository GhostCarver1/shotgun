
#include "post_game.h"

int handle_post_game_request(int client_fd, const char * request)
{
    char *body = strstr(request, "\r\n\r\n");
    
    if (!body) {
        printf("No Body Found in Login Request.\n");
        send_response(client_fd, "application/json", "{\"status\":\"failure\", \"reason\":\"no body found\"}");
        return 0;
    }
    body += 4;

    PGRequest pgrequest;
    PGContext pgcontext;
    PGResponse pgresponse;

    JsonFeild json_feilds[] = {
        FIELD(token, pgrequest),
        FIELD(user_id, pgrequest),
    };

    Result extracting_json_feilds_result = extract_json_feilds(body, json_feilds, 2);

    if (extracting_json_feilds_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_json_feilds_result.message);
        return 0;
    }

    Result extracting_names_result = extract_json_list_of_strings(body, "player_ids", MAX_PLAYERS, ID_SIZE, pgrequest.player_ids);

    if (extracting_names_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_names_result.message);
        return 0;
    } 
    
    int player_count = get_player_count(pgrequest.player_ids);
    
    if (player_count == 0)
    {
        send_failure(client_fd, 400, "PLAYER COUNT CANNOT BE 0 \n");
        return 0;
    }

    PGconn * conn = db_connect();
    Result reserve_game_id_result = db_reserve_game_id(conn, pgresponse.game_id);
    if (reserve_game_id_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, reserve_game_id_result.message);
        return 0;
    } 


    Result reserver = db_connect_game_to_player_ids(conn, player_count, pgresponse.game_id, pgrequest.player_ids);

    if (reserver.status != SUCCESS)
    {
        send_failure(client_fd, 400, reserver.message);
        return 0;
    } 

    char response[MAX_RESPONSE_SIZE];
    snprintf(response, sizeof(response), "{\"status\":\"success\", \"game_id\":\"%s\"}", pgresponse.game_id);
    send_response(client_fd, "application/json", response);
}

Result db_reserve_game_id(PGconn * conn, char game_id[ID_SIZE])
{

    const char *sql ="Insert into games DEFAULT values returning game_id;";

    const char *params[0] = {};

    PGresult *res = PQexecParams(
        conn,sql,0,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID, "Reservering game id invalid: %s\n", PQerrorMessage(conn));
    }
    if (PQntuples(res) == 0) {
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"Unable to reserve game into database: %s\n", game_id);
    }

    strncpy(game_id, PQgetvalue(res,0,0), ID_SIZE);
    game_id[ID_SIZE-1]='\0'; 

    PQclear(res);

    return create_success();
}

Result db_connect_game_to_player_ids(PGconn * conn, int player_count, char game_id[ID_SIZE], char player_ids[MAX_PLAYERS][ID_SIZE])
{
    int buffer_size = 100 + 32 * player_count;
    char sql[buffer_size];
    sql[buffer_size-1] = '\0';

    strncpy(sql, "INSERT into game_players (game_id, player_id) values ", buffer_size);

    int used = 53;

    for (int i = 0; i < player_count; i++)
    {
        char temp_string[15];
        int written = snprintf(temp_string, 15, "($1,$%d),",(i+2));
        memcpy(sql + used, temp_string, written);
        used += written;
    }

    strncpy(sql+used-1, " returning player_id;", buffer_size-used-1);
    used+=21;


    printf("SQL: %s \n", sql);

    const char *params[player_count+1];

    params[0] = game_id;

    for (int i = 0; i < player_count; i++)
    {
        params[i + 1] = player_ids[i];
    }

    printf("PARAMS AT 0 : %s \n", params[0]);

    PGresult *res = PQexecParams(
        conn,sql,player_count+1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID, "CONNECTING PLAYERS TO GAME INVALID: %s\n", PQerrorMessage(conn));
    }

    return create_success();

}

int get_player_count(char player_ids[MAX_PLAYERS][ID_SIZE])
{
    int player_count = 0;
    for (player_count = 0; player_count<MAX_PLAYERS; player_count++)
    {
        if (player_ids[player_count][0]=='\0')
        {
            break;
        }
    }
    return player_count;
}