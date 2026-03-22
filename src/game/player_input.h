#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <stdio.h>
#include <string.h>
#include "player.h"
#include "game_state.h"


void request_player_input(const char *prompt, char *buffer, size_t buffer_size);
char request_action_input(const char *prompt, const char * allowed_inputs);
Player * request_target_player(const char * action, PlayerList * player_list);

#endif