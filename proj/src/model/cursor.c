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
    return cursor;
}

/**
 * @brief Destroys cursor 
 */
void delete_cursor(Cursor *cursor){
    free(cursor);
}

void draw(Cursor *cursor){
    // TODO:
}


Sprite* cursor_get_sprite(Cursor *cursor){
    return cursor->sprite;
}

int cursor_get_x(Cursor *cursor){
    return cursor->x;
}

int cursor_get_y(Cursor *cursor){
    return cursor->x;
}
