#include "game_state.h"

bool game_state_init(GameState *game, const char creator_id[ID_SIZE])
{
    shotgun_init(&game->shotgun);
    game->player_count = 1;
    strncpy(game->current_player_id, creator_id, ID_SIZE);
    
}