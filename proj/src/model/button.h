#ifndef _LCOM_BUTTON_H_
#define _LCOM_BUTTON_H_

#include <lcom/lcf.h>

#include "../viewer/guiDrawer.h"

typedef struct{
    Sprite *sprite;
    int x;
    int y;
} Button;

Button *new_button(int x, int y, int text_index); // "Constructor"

void delete_button(Button *button); // "Destructor"

void draw_button(Button *button);

// Getters and setters
Sprite* button_get_sprite(Button *button);
int button_get_x(Button *button);
int button_get_y(Button *button);

#endif /* _BUTTON_H */
