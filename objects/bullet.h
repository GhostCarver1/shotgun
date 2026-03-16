#ifndef BULLET_H
#define BULLET_H

typedef enum {
    BULLET_DUD,
    BULLET_LIVE,
    BULLET_FIRED,
    BULLET_DOUBLE
} BulletState;

typedef struct {
    BulletState state;
} Bullet;

Bullet create_bullet();
char * get_bullet_name(BulletState bulletstate);
int double_bullet_damage(Bullet * bullet);

#endif