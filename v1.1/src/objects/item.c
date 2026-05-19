#include "item.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



Item create_item(ItemType type)
{
    Item item;
    item.type = type;
    return item;
}

int delete_item(Item * item)
{
    return 1;
}

Item create_random_item()
{
    return create_item(rand() % ITEM_TYPES);
}

char * get_item_name(ItemType type)
{
    switch (type) {
        case ITEM_BURNER_PHONE:
            return "Burner Phone\0";
        case ITEM_CIGARETTE_PACK:
            return "Cigarette Pack\0";
        case ITEM_HAND_SAW:
            return "Hand Saw\0";
        case ITEM_HANDCUFFS:
            return "Handcuffs\0";
        case ITEM_MAGNIFYING_GLASS:
            return "Magnifying Glass\0";
        case ITEM_ADRENALINE:
            return "Adrenaline\0";
        case ITEM_EXPIRED_MEDICATION:
            return "Expired Medication\0";
        case ITEM_INVERTER:
            return "Inverter\0";
        case ITEM_JAMMER:
            return "Jammer\0";
        case ITEM_REMOTE:
            return "Remote\0";
        case ITEM_BEER:
            return "Beer\0";
        default:
            return "Unknown\0";
    }
}
