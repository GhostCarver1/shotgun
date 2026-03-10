#ifndef SHOTGUN_H
#define SHOTGUN_H
#include "player_list.h"
#include "player.h"
#include "../constants.h"
#include "bullet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Shotgun Shotgun;

struct Shotgun
{
    Bullet bullets[MAX_BULLETS];
    int bullet_index;
    int bullet_count;
};

Shotgun create_shotgun();
Shotgun delete_shotgun(Shotgun *shotgun);
int reload_shotgun(Shotgun *shotgun);
int shoot_shotgun(Shotgun *shotgun, Player *target);
void print_shotgun(const Shotgun *shotgun);


#endif