#ifndef _LCOM_CURSOR_H_
#define _LCOM_CURSOR_H_

#include "../viewer/guiDrawer.h"

typedef struct{
    Sprite *sprite;
    int x; 
    int y;
} Cursor;

Cursor *new_cursor(int x, int y); // "Constructor"

void delete_cursor(Cursor *cursor); // "Destructor"

void draw(Cursor *cursor);

// Getters and setters
Sprite* cursor_get_sprite(Cursor *cursos);
int cursor_get_x(Cursor *cursor);
int cursor_get_y(Cursor *cursor);

#endif /* _CURSOR_H */
