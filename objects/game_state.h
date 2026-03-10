#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "player_list.h"
#include "shotgun.h"

typedef struct {
    // the player whos turn it currently is in
    PlayerList player_list;   // list of all players in the game   // the player whose turn it currently is// index of the current player in the player list
    Shotgun shotgun;          // the shotgun used in the game
    int turn_number;         // the current turn number

} GameState;

GameState create_game_state();
int game_state_add_player(GameState *game_state, Player player);
Player * game_state_get_current_player(const GameState *game_state);
void print_game_state(const GameState *game_state);
void do_turn(GameState *game_state);

#endif