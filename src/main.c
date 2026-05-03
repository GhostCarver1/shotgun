
#include "objects/shotgun.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game/game_state.h"
#include "connection/server.h"

int main()
{
    GameState game_state = create_game_state();
    Player al = create_player("Alice");
    Player bb = create_player("Bob");
    Player cc = create_player("Charlie");

    setup_webpage(&game_state);

    /*
    Item handcuffs = create_item(ITEM_HANDCUFFS);
    Item magnifying_glass = create_item(ITEM_MAGNIFYING_GLASS);


    add_item(&al.inventory, &magnifying_glass);
    add_item(&al.inventory, &handcuffs);

    game_state_add_player(&game_state, al);
    game_state_add_player(&game_state, bb);
    game_state_add_player(&game_state, cc);

    for (;;) {
        print_game_state(&game_state);
        print_inventory(&game_state_get_current_player(&game_state)->inventory);
        do_turn(&game_state);
    }
        */
    return 0; 
}