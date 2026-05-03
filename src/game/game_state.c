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
#include "../objects/item.h"

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
    printf("Players:\n");
    print_player_list_with_lives(&game_state->player_list);
    printf("Shotgun: ");
    print_shotgun(&game_state->shotgun);
    printf("%s... it is your turn.\n", game_state->player_list.current->player.name);
}

char * get_game_state_json(const GameState *game_state)
{
    // For simplicity, we'll just return a static JSON string here. In a real implementation, you'd want to serialize the actual game state.
    char *json = malloc(256);
    snprintf(json, 256, "{ \"turn_number\": 0, \"current_player\": \"0\" }",
        game_state->turn_number,
        game_state->player_list.current ? game_state->player_list.current->player.name : "None"
    );
    return json;
}














