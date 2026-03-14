#ifndef SHOTGUN_PLAYER_LIST_H
#define SHOTGUN_PLAYER_LIST_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "player.h"

typedef struct PlayerList PlayerList;
typedef struct PlayerLink PlayerLink;

struct PlayerLink
{
    PlayerLink *previous;
    Player player;
    PlayerLink *next;
};

struct PlayerList
{
    PlayerLink *current;
    PlayerLink *first;
    size_t count;
    PlayerLink *last;
};

Player * get_current_player(const PlayerList *player_list);
int add_player(PlayerList *player_list, Player player);
Player * get_player(const PlayerList *player_list, const char *name);
void print_player_list(const PlayerList *player_list);
PlayerList create_player_list();
Player * advance_current_player(PlayerList *player_list);


#endif