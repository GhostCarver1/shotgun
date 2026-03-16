#include "player.h"
#include "player_list.h"
#include "inventory.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Player create_player(const char* name)
{
    if (strlen(name) > MAX_NAME_LENGTH-1) {
        fprintf(stderr, "Name too long: %s\n", name);
        exit(EXIT_FAILURE);
    }
    Player p;
    p.lives = MAX_LIVES;
    strncpy(p.name, name, MAX_NAME_LENGTH);
    p.name[MAX_NAME_LENGTH - 1] = '\0';
    p.inventory = create_inventory();
    p.notes = create_notes();
    p.handcuffed = 0;
    return p;
}

Player delete_player(const Player* player)
{
    Player p;
    p.name[0] = '\0';
    p.lives = 0;
    return p;
}

int player_is_alive(const Player* player)
{
    return player->lives > 0;
}

void player_add_note(Player * player, const Note note)
{
    player->notes.note_list[player->notes.note_count] = note;
    player->notes.note_count++;
}

void hurt_player(Player *player, int damage)
{
    player->lives-=damage;
    if (player->lives < 0)
    {
        player->lives = 0;
    }
}
