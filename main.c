
#include "objects/shotgun.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game/game_state.h"

int main()
{
    GameState game_state = create_game_state();
    Player al = create_player("Alice");
    Player bb = create_player("Bob");
    Player cc = create_player("Charlie");

    add_random_item(&al.inventory);
    add_random_item(&bb.inventory);
    add_random_item(&cc.inventory);
    add_random_item(&al.inventory);
    add_random_item(&bb.inventory);
    add_random_item(&cc.inventory);
    add_random_item(&al.inventory);
    add_random_item(&bb.inventory);
    add_random_item(&cc.inventory);
    add_random_item(&al.inventory);
    add_random_item(&bb.inventory);
    add_random_item(&cc.inventory);
    add_random_item(&al.inventory);
    add_random_item(&bb.inventory);
    add_random_item(&cc.inventory);
    add_random_item(&al.inventory);
    add_random_item(&bb.inventory);
    add_random_item(&cc.inventory);

    game_state_add_player(&game_state, al);
    game_state_add_player(&game_state, bb);
    game_state_add_player(&game_state, cc);

    print_game_state(&game_state);
    for (;;) {
        do_turn(&game_state);
        print_game_state(&game_state);
    }
    return 0;
}