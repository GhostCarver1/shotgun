#include "inventory.h"
#include "../objects/item.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player_input.h"
#include "game_state.h"
#include "player.h"

void request_player_input(const char *prompt, char *buffer, size_t buffer_size)
{
    printf("%s", prompt);
    fgets(buffer, buffer_size, stdin);
    buffer[strcspn(buffer, "\n")] = 0; 
}

char request_action_input(const char *prompt, const char * allowed_inputs)
{
    
    char buffer[128];
    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return '\0';
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strchr(allowed_inputs, buffer[0]) != NULL && buffer[1]=='\0' && buffer[0]!='\0')
        {
            return buffer[0];
        }

        printf("Invalid input: %s\n", buffer);
    }
}

Player * request_target_player(const char * action, PlayerList * player_list)
{

    char player_name[MAX_NAME_LENGTH];
    while (1)
    {
        printf("select a player to %s: \n", action);
        print_player_list(player_list);
        fgets(player_name, MAX_NAME_LENGTH, stdin);
        player_name[strcspn(player_name, "\n")] = '\0';
        Player * target = get_player(player_list, player_name);
        if (target == NULL)
        {
            printf("player \"%s\" not found. try again.\n", player_name);
        }
        else
        {
            return target;
        }
    }
    return NULL;
}

