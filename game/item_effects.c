#include "game_state.h"
#include "player.h"
#include "../objects/item.h"
#include "../objects/notes.h"
#include "item_effects.h"
#include "../objects/bullet.h"

int use_item(GameState *game_state, Player *player, Item *item)
{
    switch (item->type) {
        case ITEM_BURNER_PHONE:
            printf("%s uses a Burner Phone. The player has new information\n", player->name);
            use_burner_phone(player, &game_state->shotgun);
            break;
        case ITEM_CIGARETTE_PACK:
            printf("%s uses a Cigarette Pack. The player has more health\n", player->name);
            use_cigarette_pack(player);
            break;
        case ITEM_HAND_SAW:
            printf("%s uses a Hand Saw. The next bullet, if its not a dud, is double damage\n", player->name);
            use_hand_saw(player, &game_state->shotgun);
            break;
        case ITEM_HANDCUFFS:
            printf("%s uses Handcuffs. It does nothing.\n", player->name);
            break;
        case ITEM_MAGNIFYING_GLASS:
            printf("%s uses a Magnifying Glass. It does nothing.\n", player->name);
            break;
        case ITEM_ADRENALINE:
            printf("%s uses Adrenaline. It does nothing.\n", player->name);
            break;
        case ITEM_EXPIRED_MEDICATION:
            printf("%s uses Expired Medication. It does nothing.\n", player->name);

            break;
        case ITEM_INVERTER:
            printf("%s uses an Inverter. It does nothing.\n", player->name);
            break;
        case ITEM_JAMMER:
            printf("%s uses a Jammer. It does nothing.\n", player->name);
            break;
        case ITEM_REMOTE:
            printf("%s uses a Remote. It does nothing.\n", player->name);
            break;
        case ITEM_BEER:
            printf("%s drinks a Beer and feels refreshed!\n", player->name);
            break;
        default:
            fprintf(stderr, "Unknown item type: %d\n", item->type);
    }
    return 0;
}

int use_burner_phone(Player *player, Shotgun * shotgun)
{
    if (shotgun->bullet_count==0)
    {
        return -1;
    }
        
    int bullet_index = rand() % shotgun->bullet_count + shotgun->bullet_index;
    char * bullet_name = get_bullet_name(shotgun->bullets[bullet_index].state);
    char text[MAX_NOTE_TEXT_LENGTH];
    snprintf(text,sizeof(text), "the shell in %d rounds will be a %s",bullet_index,bullet_name);
    Note note = create_note(text);
    player_add_note(player, note);
    printf("A note was given to the player {%s}",text);
    return 1;
}

int use_cigarette_pack(Player *player)
{
    player->lives+=1;
    return 1;
}

int use_hand_saw(Player * player, Shotgun * shotgun)
{
    Bullet * next_bullet = &shotgun->bullets[shotgun->bullet_index];
    double_bullet_damage(next_bullet);
}