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

int add_random_item(Inventory *inventory)
{
    if (inventory->item_count >= MAX_ITEMS) {
        fprintf(stderr, "Inventory is full\n");
        return -1;
    }
    Item item = create_random_item();
    inventory->items[inventory->item_count] = item;
    inventory->item_count++;
    return 1;
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

Item * get_item_from_name(Inventory * inventory, char * item_name)
{
    for (int i = 0; i<inventory->item_count; i++){
        Item * current_item = &inventory->items[i];
        if (strcmp(item_name, get_item_name(current_item->type))==0)
            return current_item;
    }
    return NULL;
}

int remove_item(Inventory * inventory, Item * item)
{
    for (int i = 0; i<inventory->item_count; i++) {
        if (item == &inventory->items[i])
        {
            inventory->items[i] = inventory->items[inventory->item_count - 1];
            delete_item(item);
            inventory->item_count--;
            return 1;
        }
    }
    return -1;
}