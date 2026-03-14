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