#ifndef BULLET_H
#define BULLET_H

typedef enum {
    BULLET_DUD,
    BULLET_LIVE,
    BULLET_FIRED
} BulletState;

typedef struct {
    BulletState state;
} Bullet;

Bullet create_bullet();

#endif