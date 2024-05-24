#ifndef _LCOM_MAINROOM_H_
#define _LCOM_MAINROOM_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../cursor.h"

typedef struct{
    Button *startNewButton;
    Button *continueButton;
    Button *quitButton;
    Cursor *cursor;
} MainRoom;

MainRoom *new_mainRoom(); // "Constructor"

void delete_mainRoom(MainRoom *mainRoom); // "Destructor"

void draw_mainRoom(MainRoom *mainRoom);

// Getters and setters
Button* mainRoom_get_startNewButton(MainRoom *mainRoom);
Button* mainRoom_get_continueButton(MainRoom *mainRoom);
Button* mainRoom_get_quitButton(MainRoom *mainRoom);
Cursor* mainRoom_get_cursor(MainRoom *mainRoom);

#endif /* _MAINROOM_H */
