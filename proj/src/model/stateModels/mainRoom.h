#ifndef _LCOM_MAINROOM_H_
#define _LCOM_MAINROOM_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../cursor.h"
#include "../minigotchi.h"

typedef struct{
    Button *miniGamesButton;
    Button *quitButton;
    Minigotchi* minigotchi;
    Cursor *cursor;
} MainRoom;

MainRoom *new_mainRoom(); // "Constructor"

void delete_mainRoom(MainRoom *mainRoom);

Minigotchi* mainRoom_get_minigotchi(MainRoom *mainRoom);
Button* mainRoom_get_miniGamesButton(MainRoom *mainRoom);
Button* mainRoom_get_quitButton(MainRoom *mainRoom);
Cursor* mainRoom_get_cursor(MainRoom *mainRoom);

#endif /* _MAINROOM_H */
