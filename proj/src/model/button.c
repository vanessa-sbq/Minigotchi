#include "button.h"

Button *new_button(int x, int y) {
     Button *button = malloc(sizeof(*button));
    if (button == NULL) return NULL;

    // Initialize Cursor
    button->sprite = get_button_sprite();
    button->x = x; 
    button->y = y;


    return button;
}

void delete_button(Button *button) {
    free(button);
}

void draw_button(Button *button) {
    drawSprite(button->sprite, button->x, button->y);
}

// Getters and setters
Sprite* button_get_sprite(Button *button) {
    return button->sprite;
}

int button_get_x(Button *button) {
    return button->x;
}

int button_get_y(Button *button) {
    return button->x;
}
