#ifndef ITEM_EFFECTS_H
#define ITEM_EFFECTS_H

#include "game_state.h"
#include "player.h"
#include "../objects/item.h"

int use_item(GameState *game_state, Player *player, Item *item);
int use_burner_phone(Player *player, Shotgun * shotgun);
int use_hand_saw(Player * player, Shotgun * shotgun);
int use_cigarette_pack(Player * player);

#endif