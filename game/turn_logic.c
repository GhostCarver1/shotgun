#include "player.h"
#include "player_list.h"
#include "inventory.h"
#include "../constants.h"
#include "game_state.h"
#include "../objects/item.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "turn_logic.h"
#include "player_input.h"


void do_turn(GameState *game_state)
{
    Player *current_player = game_state_get_current_player(game_state);

    if (!player_is_alive(current_player)) {
        fprintf(stderr, "%s is dead and cannot take a turn\n", current_player->name);
        advance_current_player(&game_state->player_list);
        return;
    }

    if (player_is_handcuffed(current_player))

    char action = request_action_input("please choose action ((s)hoot/(u)se item):", "us");

    if (action == 'u') {
        handle_use_item_action(game_state);
        action = request_action_input("please enter (s) to shoot:", "s");
    }

    if (action == 's') 
    {
        handle_shoot_action(game_state);
        return;
    }

}