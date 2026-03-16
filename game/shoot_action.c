#include "game_state.h"
#include "player_input.h"

void handle_shoot_action(GameState *game_state)
{
    printf("***********************************************\n");
    Player * current_player = game_state_get_current_player(game_state);
    
    Player *target_player = request_target_player("shoot", &game_state->player_list);

    printf("%s shoots at %s\n", current_player->name, target_player->name);

    shoot_shotgun(&game_state->shotgun, target_player);
    game_state->turn_number++;
    advance_current_player(&game_state->player_list);

    printf("***********************************************\n");
}