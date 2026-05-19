
#ifndef PLAYER_RESPONSE_H
#define PLAYER_RESPONSE_H

#include "../game/player_list.h"

typedef struct {
    PlayerList * player_list;
} PlayerListResponse;

char * get_player_list_json(const PlayerList *player_list);
void send_player_list(int client_fd, const PlayerList *player_list);


#endif