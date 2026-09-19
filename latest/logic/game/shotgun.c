#include "shotgun.h"

bool shotgun_init(Shotgun * shotgun)
{
    shotgun->bullet_count = MAX_BULLETS;
    shotgun->bullet_index = MAX_BULLETS - 1;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        shotgun->bullets[i].state = BULLET_LIVE;
    }
    
}