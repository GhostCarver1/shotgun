#ifndef SHOTGUN_PLAYER_H
#define SHOTGUN_PLAYER_H
#include <string.h>
#include "../constants.h"
#include "inventory.h"
#include "../objects/notes.h"

typedef struct Player Player;

struct Player
{
    char name[MAX_NAME_LENGTH];   
    int lives;
    Inventory inventory;
    Notes notes;
    int handcuffed;
};

Player create_player(const char* name);
Player delete_player(const Player* player);
int player_is_alive(const Player* player);
void player_add_note(Player * player, const Note note);
void hurt_player(Player *player, int damage);

#endif