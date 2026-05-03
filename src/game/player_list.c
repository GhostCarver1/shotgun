#include "player.h"
#include "player_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player_list.h"
#include "player.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PlayerList create_player_list()
{

    PlayerList player_list;
    player_list.current = NULL;
    player_list.first = NULL;
    player_list.count = 0;
    player_list.last = NULL;
    return player_list;
}

int add_player(PlayerList *player_list, Player player)
{

    if (player_list->count >= MAX_PLAYERS) {
        fprintf(stderr, "Player list is full\n");
        return 0;
    }
    PlayerLink *link = malloc(sizeof(PlayerLink));
    if (player_list->count==0)
    {
        link->previous = link;
        link->player = player;
        link->next = link;
        player_list->first = link;
        player_list->last = link;
        player_list->current = link;
    }
    else
    {
        link->previous = player_list->last;
        link->player = player;
        link->next = player_list->first;
        player_list->last->next = link;
        player_list->first->previous = link;
        player_list->last = link;
    }
    player_list->count++;
    return 1;
}

Player * get_player(const PlayerList *player_list, const char* name)
{
    PlayerLink *current = player_list->first;
    for (size_t i = 0; i < player_list->count; i++) {
        if (strcmp(current->player.name, name) == 0) {
            return &current->player;
        }
        current = current->next;
    }
    return NULL;
}

Player * get_current_player(const PlayerList *player_list)
{
    if (player_list->current == NULL) {
        fprintf(stderr, "No players in the list\n");
        exit(EXIT_FAILURE);
    }
    return &player_list->current->player;
}

Player * advance_current_player(PlayerList *player_list)
{
    if (player_list->current == NULL) {
        fprintf(stderr, "No players in the list\n");
        exit(EXIT_FAILURE);
    }
    player_list->current = player_list->current->next;
    return &player_list->current->player;
}


void print_player_list(const PlayerList *player_list)
{
    PlayerLink *iterator = player_list->first;
    for (size_t i = 0; i < player_list->count; i++) {
        printf(" - %s\n", iterator->player.name);
        iterator = iterator->next;
}
    }
    

void print_player_list_with_lives(const PlayerList *player_list)
{
    PlayerLink *iterator = player_list->first;
    for (size_t i = 0; i < player_list->count; i++) {
        printf(" - %s\n", iterator->player.name);
        if (iterator->player.lives <= 0) {
            printf(" [RIP]\n");
        }
        else {
            printf(" [");
            for (int i = 0; i < iterator->player.lives; i++) {
                printf(" <3");
            }
            printf(" ]\n");
        }
        iterator = iterator->next;
    }
}

void print_player_list_with_items(const PlayerList * player_list)
{
    PlayerLink * iterator = player_list-> first;
    for (size_t i = 0; i < player_list->count; i++) {
        printf(" - %s\n", iterator->player.name);
        print_inventory(&iterator->player.inventory);
    }
}

