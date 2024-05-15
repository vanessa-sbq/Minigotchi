#ifndef _LCOM_CURSOR_H_
#define _LCOM_CURSOR_H_

#include "../viewer/guiDrawer.h"

typedef struct{
    Sprite *sprite;
    int x; 
    int y;
} Cursor;

Cursor *new_cursor(uint32_t x, uint32_t y); // "Constructor"

void delete_cursor(Cursor *cursor); // "Destructor"

#endif /* _CURSOR_H */
