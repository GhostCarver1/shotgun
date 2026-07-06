
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

    Result extracting_names_result = extract_json_list_of_strings(body, "player_ids", MAX_PLAYERS, MAX_NAME_LENGTH, pgrequest.player_ids);

    printf("player:%s\n", pgrequest.player_ids[0]);
    printf("player:%s\n", pgrequest.player_ids[1]);
    printf("player:%s\n", pgrequest.player_ids[2]);

    if (extracting_names_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_names_result.message);
        return 0;
    } 

    int player_count = 0;
    for (player_count = 0; player_count<MAX_PLAYERS; player_count++)
    {
        if (pgrequest.player_ids[player_count][0]=='\0')
        {
            break;
        }
    }

    
    printf("player:%s\n", pgrequest.player_ids[0]);
    printf("player:%s\n", pgrequest.player_ids[1]);
    printf("player:%s\n", pgrequest.player_ids[2]);

    printf("player count:%d\n",player_count);

    PGconn * conn = db_connect();
    Result reserve_game_id_result = db_reserve_game_id(conn, pgresponse.game_id);
    if (reserve_game_id_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, reserve_game_id_result.message);
        return 0;
    } 
  

    /*Result reserver = db_connect_game_id_to_player_id(conn, pgresponse.game_id, pgrequest.player_ids[1]);

    if (reserver.status != SUCCESS)
    {
        send_failure(client_fd, 400, reserver.message);
        return 0;
    } 
    
    Result reserver = db_connect_game_id_to_player_ids(conn, pgresponse.game_id, pgrequest.player_ids, player_count);
    if (reserver.status != SUCCESS)
    {
        send_failure(client_fd, 400, reserver.message);
        return 0;
    } */
    db_disconnect(conn);


    printf("player:%s\n", pgrequest.player_ids[0]);
    printf("player:%s\n", pgrequest.player_ids[1]);
    printf("player:%s\n", pgrequest.player_ids[2]);

    
    /*
    GameState game_state = create_game_state();
    Player player = create_player("Jessica");
    game_state_add_player(&game_state, player);
    print_game_state(&game_state); 
    */

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