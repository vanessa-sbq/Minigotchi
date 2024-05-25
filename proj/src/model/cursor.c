#include "cursor.h"

/**
 * @brief Constructs a cursor in the specified x and y positions 
 */
Cursor *new_cursor(int x, int y){
    // Allocate Cursor object
    Cursor *cursor = malloc(sizeof(*cursor));
    if (cursor == NULL) return NULL;

    // Initialize Cursor
    cursor->sprite = get_cursor_sprite();
    cursor->x = x; 
    cursor->y = y;
    cursor->isClicked = false;

    return cursor;
}

/**
 * @brief Destroys cursor 
 */
void delete_cursor(Cursor *cursor){
    if (cursor != NULL){
        free(cursor);
    }
}

void draw_cursor(Cursor *cursor){
    drawSprite(cursor->sprite, cursor->x, cursor->y);
}

void set_buttonClicked(bool isPressed, Cursor* cursor) {
    cursor->isClicked = isPressed;
}

bool get_buttonClicked(Cursor* cursor) {
    return cursor->isClicked;
}


Sprite* cursor_get_sprite(Cursor *cursor){
    return cursor->sprite;
}

int cursor_get_x(Cursor *cursor){
    return  cursor->x;
}

int cursor_get_y(Cursor *cursor){
    return cursor->y;
}

bool cursor_isClicked(Cursor *cursor){
    return cursor->isClicked;
}

void cursor_set_x(Cursor *cursor, int x){
    cursor->x = x;
}

void cursor_set_y(Cursor *cursor, int y){
    cursor->y = y;
}
