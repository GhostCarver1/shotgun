#ifndef PLAYER_INVENTORY_H
#define PLAYER_INVENTORY_H


# include "../constants.h"
# include <stdio.h>
# include <stdlib.h>
# include "../objects/item.h"

typedef struct {
    Item items[MAX_ITEMS];
    size_t item_count;
} Inventory;

Inventory create_inventory();
int add_random_item(Inventory *inventory);
int remove_item(Inventory * inventory, Item * item);
void print_inventory(const Inventory *inventory);
Item * get_item_from_name(Inventory * inventory, char * item_name);
int add_item(Inventory *inventory, Item * item);

#endif