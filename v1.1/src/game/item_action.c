#include "game_state.h"
#include "../constants.h"
#include "item_action.h"
#include "player_input.h"
#include "item_effects.h"
#include "inventory.h"

void handle_use_item_action(GameState *game_state)
{
    printf("***********************************************\n");
    char item_name[MAX_PLAYER_INPUT];
    Item * item = NULL;
    Player * current_player = game_state_get_current_player(game_state);
    print_inventory(&current_player->inventory);
    for (; item == NULL;)
    {
        request_player_input("select the item you want to use:", item_name, MAX_PLAYER_INPUT);
        item = get_item_from_name(&current_player->inventory,item_name);
        if (item != NULL)
        {
            printf("you chose: %s\n", get_item_name(item->type));
        }
        if (item == NULL) 
        {
            fprintf(stderr, "Item not found: %s. Try again\n", item_name);
        }
    }
    use_item(game_state, current_player, item);
    remove_item(&current_player->inventory, item);
    printf("***********************************************\n");
}

