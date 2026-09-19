#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "../logic_constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Shotgun Shotgun;

typedef enum {
    BULLET_DUD,
    BULLET_LIVE,
    BULLET_FIRED,
    BULLET_DOUBLE
} BulletState;

typedef struct {
    BulletState state;
} Bullet;

struct Shotgun
{
    Bullet bullets[MAX_BULLETS];
    int bullet_index;
    int bullet_count;
};





bool shotgun_init(Shotgun * shotgun);

#endif