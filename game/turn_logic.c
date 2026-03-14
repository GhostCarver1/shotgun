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
void request_player_input(const char *prompt, char *buffer)
{
    printf("%s", prompt);
    fgets(buffer, MAX_PLAYER_INPUT, stdin);
    buffer[strcspn(buffer, "\n")] = 0; 
}

void do_turn(GameState *game_state)
{
    Player *current_player = game_state_get_current_player(game_state);

    if (!player_is_alive(current_player)) {
        fprintf(stderr, "%s is dead and cannot take a turn\n", current_player->name);
        advance_current_player(&game_state->player_list);
        return;
    }

    char * action_prompt = "Enter action ((s)hoot/(u)se item): ";

    char action[MAX_PLAYER_INPUT];

    printf("%s's turn\n", current_player->name);

    request_player_input(action_prompt, action);

    for (; strcmp(action, "s") != 0 && strcmp(action, "u") != 0;) {
        fprintf(stderr, "Invalid action: %s\n", action);
        request_player_input(action_prompt, action);
    }

    while (strcmp(action, "u") == 0) {
        handle_use_item_action(game_state);
        request_player_input(action_prompt, action);
    }

    if (strcmp(action, "s") == 0) 
    {
        handle_shoot_action(game_state);
        return;
    }

    return;
}

void handle_shoot_action(GameState *game_state)
{
    Player * current_player = game_state_get_current_player(game_state);
    char target_player_name[MAX_PLAYER_INPUT];
    request_player_input("player you want to shoot: ", target_player_name);
    Player *target_player = get_player(&game_state->player_list, target_player_name);

    for (; target_player == NULL;) 
    {
        fprintf(stderr, "Player not found: %s. Try again\n", target_player_name);
        request_player_input("player you want to shoot: ", target_player_name);
        target_player = get_player(&game_state->player_list, target_player_name);
    }

    printf("%s shoots at %s\n", current_player->name, target_player->name);
    shoot_shotgun(&game_state->shotgun, target_player);
    game_state->turn_number++;
    advance_current_player(&game_state->player_list);
}

void handle_use_item_action(GameState *game_state)
{
    char item_name[MAX_PLAYER_INPUT];
    Item * item = NULL;
    Player * current_player = game_state_get_current_player(game_state);
    print_inventory(&current_player->inventory);
    for (; item == NULL;)
    {
        request_player_input("select the item you want to use:", item_name);
        item = get_item_from_player(current_player, item_name);
        printf("%s you choose", get_item_name(item->type));
    }
    return;
}

Item * get_item_from_player(Player * player, char * item_name)
{
    for (int i = 0; i<player->inventory.item_count; i++){
        Item * current_item = &player->inventory.items[i];
        if (strcmp(item_name, get_item_name(current_item->type))==0)
            return current_item;
    }
    return NULL;
}