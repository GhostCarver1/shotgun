
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

    Result db_ensure_players_exist_result = db_ensure_existance_players(conn, player_count, pgrequest.player_ids);
    if (db_ensure_players_exist_result.status != SUCCESS)
    {
        db_disconnect(conn);
        send_failure(client_fd, 400, db_ensure_players_exist_result.message);
        return 0;
    } 

    Result db_reserve_game_id_result = db_reserve_game_id(conn, pgrequest.user_id  ,pgresponse.game_id);
    if (db_reserve_game_id_result.status != SUCCESS)
    {
        db_disconnect(conn);
        send_failure(client_fd, 400, db_reserve_game_id_result.message);
        return 0;
    } 

    Result db_connect_game_to_player_id_result = db_connect_game_to_player_ids(conn, player_count, pgresponse.game_id, pgrequest.player_ids);
    if (db_connect_game_to_player_id_result.status != SUCCESS)
    {
        db_disconnect(conn);
        send_failure(client_fd, 400, db_connect_game_to_player_id_result.message);
        return 0;
    } 

    db_disconnect(conn);

    char response[MAX_RESPONSE_SIZE];
    
    snprintf(response, sizeof(response), "{\"status\":\"success\", \"game_id\":\"%s\"}", pgresponse.game_id);
    send_response(client_fd, "application/json", response);
}

Result db_reserve_game_id(PGconn * conn, const char owner_id[ID_SIZE],  char game_id[ID_SIZE])
{

    const char *sql ="Insert into games (owner_id) values ($1) returning game_id;";

    const char *params[1] = {owner_id};

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID, "Reservering game id invalid: %s\n", PQerrorMessage(conn));
    }
    if (PQntuples(res) == 0) {
        PQclear(res);
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
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID, "CONNECTING PLAYERS TO GAME INVALID: %s\n", PQerrorMessage(conn));
    }

    PQclear(res);
    return create_success();

}

Result db_ensure_existance_players(PGconn * conn, int player_count, char player_ids[MAX_PLAYERS][ID_SIZE])
{
    int buffer_size = 100 + 32 * player_count;
    char sql[buffer_size];
    sql[buffer_size-1] = '\0';

    strncpy(sql, "select count(*) from players where player_id IN (", buffer_size);

    int used = 49;

    for (int i = 0; i < player_count; i++)
    {
        char temp_string[20];
        int written = snprintf(temp_string, 20, "$%d,",(i+1));
        memcpy(sql + used, temp_string, written);
        used += written;
    }

    strncpy(sql+used-1, ");", buffer_size-used-1);
    used+=5;

    printf("SQL: %s \n", sql);

    const char *params[player_count];

    for (int i = 0; i < player_count; i++)
    {
        params[i] = player_ids[i];
    }

    PGresult *res = PQexecParams(
        conn,sql,player_count,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID, "Reservering game id invalid: %s\n", PQerrorMessage(conn));
    }
    if (PQntuples(res) == 0) {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_EMPTY,"Unable to reserve game into:  %s\n", PQerrorMessage(conn));
    }

    int found_players = atoi(PQgetvalue(res,0,0));
    printf("PLAYERS: %d \n", found_players);

    if (found_players!=player_count)
    {
        PQclear(res);
        return create_error(ERROR_TYPE_DATABASE,ERROR_CODE_DATABASE_QUERY_INVALID,"One of the player id entered does not exist");
    }
    
    PQclear(res);
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