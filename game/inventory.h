# include "../constants.h"
# include <stdio.h>
# include <stdlib.h>
# include "../objects/item.h"

#ifndef PLAYER_INVENTORY_H
#define PLAYER_INVENTORY_H

typedef struct {
    Item items[MAX_ITEMS];
    size_t item_count;
} Inventory;

Inventory create_inventory();
void add_random_item(Inventory *inventory);
int remove_item(Inventory *inventory, ItemType type);
void print_inventory(const Inventory *inventory);
#endif