#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player_list.h"
#include "player.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_state.h"

GameState create_game_state()
{
    GameState game_state;
    game_state.player_list = create_player_list();
    game_state.shotgun = create_shotgun();
    game_state.turn_number = 0;
    return game_state;
}

int game_state_add_player(GameState *game_state, Player player)
{
    return add_player(&game_state->player_list, player);
}

Player * game_state_get_current_player(const GameState *game_state)
{
    return &game_state->player_list.current->player;
}

void print_game_state(const GameState *game_state)
{
    printf("Turn %d\n", game_state->turn_number);
    printf("Current player: %s\n", game_state->player_list.current->player.name);
    printf("Players:\n");
    print_player_list(&game_state->player_list);
    printf("Shotgun: ");
    print_shotgun(&game_state->shotgun);
}

void request_player_input(const char *prompt, char *buffer)
{
    printf("%s", prompt);
    scanf("%s", buffer);
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

    char action[MAX_NAME_LENGTH];

    printf("%s's turn\n", current_player->name);

    request_player_input(action_prompt, action);

    for (; strcmp(action, "s") != 0 && strcmp(action, "u") != 0;) {
        fprintf(stderr, "Invalid action: %s\n", action);
        request_player_input(action_prompt, action);
    }

    if (strcmp(action, "u") == 0) {
        fprintf(stderr, "Item usage not implemented yet\n");
        strcpy(action, "s");
    }

    if (strcmp(action, "s") == 0) 
    {
        char target_player_name[MAX_NAME_LENGTH];
        request_player_input("player you want to shoot: ", target_player_name);
        Player *target_player = get_player(&game_state->player_list, target_player_name);

        for (; target_player == NULL;) {
            fprintf(stderr, "Player not found: %s. Try again\n", target_player_name);
            request_player_input("player you want to shoot: ", target_player_name);
            target_player = get_player(&game_state->player_list, target_player_name);
        }

        printf("%s shoots at %s\n", current_player->name, target_player->name);
        shoot_shotgun(&game_state->shotgun, target_player);
        game_state->turn_number++;
        advance_current_player(&game_state->player_list);
        return;
    }
}