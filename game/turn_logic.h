#ifndef TURN_LOGIC_H
#define TURN_LOGIC_H

void handle_use_item_action(GameState *game_state);
void handle_shoot_action(GameState *game_state);
void do_turn(GameState *game_state);
void request_player_input(const char *prompt, char *buffer);
Item * get_item_from_player(Player * player, char * item_name);

#endif