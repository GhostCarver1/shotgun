#include "../game/player.h"
#include "../game/player_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../game/player_list.h"
#include "../game/player.h"
#include "shotgun.h"
#include "bullet.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Shotgun create_shotgun()
{
    Shotgun shotgun;
    reload_shotgun(&shotgun);
    return shotgun;
}

Shotgun delete_shotgun(Shotgun *shotgun)
{
    Shotgun s;
    s.bullet_index = 0;
    return s;
}

int shoot_shotgun(Shotgun *shotgun, Player *target)
{
    if (shotgun->bullet_index >= MAX_BULLETS) {
        fprintf(stderr, "No bullets left to shoot\n");
        return 0;
    }
    if (target->lives <= 0) {
        fprintf(stderr, "Target is already dead\n");
        return 0;
    }
    Bullet *bullet = &shotgun->bullets[shotgun->bullet_index];
    shotgun->bullet_index++;
    shotgun->bullet_count--;
    
    if (bullet->state == BULLET_LIVE) {
        hurt_player(target,1);
    }
    if (bullet->state == BULLET_DOUBLE) {
        hurt_player(target,2);
    }
    
    bullet->state = BULLET_FIRED;
    return 1;
}

int reload_shotgun(Shotgun *shotgun)
{
    shotgun->bullet_index = 0;
    shotgun->bullet_count = MAX_BULLETS;
    for (int i = 0; i < MAX_BULLETS; i++) {
        shotgun->bullets[i] = create_bullet();
    }
    return 1;
}

void print_shotgun(const Shotgun *shotgun)
{
    printf("[ ");
    for (int i = 0; i < MAX_BULLETS; i++) {
        const char *symbols[] = { " X ", " I ", " . " };
        printf("%s", symbols[shotgun->bullets[i].state]);
    }
    printf(" ]\n");
}   

