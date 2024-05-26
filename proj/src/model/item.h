#ifndef _ITEM_H_
#define _ITEM_H_

#include "../viewer/guiDrawer.h"

typedef enum {BANANA, APPLE, CHOCOLATE} item_t;

typedef struct{
    Sprite *sprite;
    int x;
    int y;
    item_t type;
    int quantity;
} Item;

Item *new_item(int x, int y, item_t type, int quantity);

// Getters
int item_get_x(Item *item);
int item_get_y(Item *item);
item_t item_get_type(Item *item);
int item_get_quantity(Item *item);

// Setters
void item_set_x(Item *item, int x);
void item_set_y(Item *item, int y);
void item_set_type(Item *item, item_t type);
void item_set_quantity(Item *item, int quantity);

#endif /* _ITEM_H_ */
