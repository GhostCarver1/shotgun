#ifndef SHOTGUN_PLAYER_H
#define SHOTGUN_PLAYER_H
#include <string.h>
#include "../constants.h"
#include "inventory.h"

typedef struct Player Player;

struct Player
{
    char name[MAX_NAME_LENGTH];   
    int lives;
    Inventory inventory;
};

Player create_player(const char* name);
Player delete_player(const Player* player);
int player_is_alive(const Player* player);

#endif