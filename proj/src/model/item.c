#include "item.h"

Item *new_item(int x, int y, item_t type, int quantity){
    Item *item = malloc(sizeof(*item));
    if (item == NULL) return NULL;

    //item->sprite = guiDrawer_get_item_sprite(type); // FIXME: Pass the type to get the corresponding sprite
    item->sprite = guiDrawer_get_hotbar_select_sprite(); 
    item->x = x;
    item->y = y;
    item->type = type;
    item->quantity = quantity;

    return item;
}


// Getters
int item_get_x(Item *item){
    return item->x;
}

int item_get_y(Item *item){
    return item->y;
}

item_t item_get_type(Item *item){
    return item->type;
}

int item_get_quantity(Item *item){
    return item->quantity;
}


// Setters
void item_set_x(Item *item, int x){
    item->x = x;
}

void item_set_y(Item *item, int y){
    item->y = y;
}

void item_set_type(Item *item, item_t type){
    item->type = type;
}

void item_set_quantity(Item *item, int quantity){
    item->quantity = quantity;
}
