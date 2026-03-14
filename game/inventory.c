#include "inventory.h"
#include "../objects/item.h"
#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Inventory create_inventory()
{
    Inventory inventory;
    inventory.item_count = 0;
    return inventory;
}

void add_random_item(Inventory *inventory)
{
    if (inventory->item_count >= MAX_ITEMS) {
        fprintf(stderr, "Inventory is full\n");
        return;
    }
    Item item = create_random_item();
    inventory->items[inventory->item_count] = item;
    inventory->item_count++;
}

void print_inventory(const Inventory *inventory)
{
    printf("Inventory: [");
    for (size_t i = 0; i < inventory->item_count; i++) {
        printf("(%d) %s",inventory->items[i].type, get_item_name(inventory->items[i].type));
        if (i < inventory->item_count - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}