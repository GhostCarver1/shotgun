#include "../game/player.h"
#include "../game/player_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bullet.h"

Bullet create_bullet()
{
    Bullet bullet;
    bullet.state = (BulletState)(rand() % 2);
    return bullet;
}

char * get_bullet_name(BulletState bulletstate)
{
    switch (bulletstate) {
        case BULLET_DUD:
            return "DUD\0";
        case BULLET_FIRED:
            return "FIRED\0";
        case BULLET_LIVE:
            return "LIVE\0";
        default:
            return "UNKNOWN\0";
    };
}

int double_bullet_damage(Bullet * bullet)
{
    if (bullet->state==BULLET_LIVE)
    {
        bullet->state=BULLET_DOUBLE;
        return 1;
    }
    return 0;
}