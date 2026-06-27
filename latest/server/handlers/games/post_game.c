
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
        FIELD(id, pgrequest),
    };

    Result extracting_json_feilds_result = extract_json_feilds(body, json_feilds, 2);

    if (extracting_json_feilds_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_json_feilds_result.message);
        return 0;
    }

    /*Result extracting_names_result = extract_json_list_of_strings(body, "player_name", pgrequest.player_names, MAX_PLAYERS, MAX_NAME_LENGTH);

    if (extracting_names_result.status != SUCCESS)
    {
        send_failure(client_fd, 400, extracting_names_result.message);
        return 0;
    }  */

    //printf("player:%s\n", pgrequest.player_names[0]);

    GameState game_state = create_game_state();
    Player player = create_player("Jessica");
    game_state_add_player(&game_state, player);

    print_game_state(&game_state);

    send_response(client_fd, "application/json", "{\"status\":\"success\"}");

}