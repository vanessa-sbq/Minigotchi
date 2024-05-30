#include <lcom/lcf.h>

#include "../../model/stateModels/mainRoom.h"
#include "../../model/button.h"
#include "../../model/cursor.h"
#include "../../player_data/database.h"

#include "../../viewer/menus/mainRoomViewer.h"
#include "../../viewer/guiDrawer.h"

typedef enum {QUIT_MAINROOM, MINIGAMES_MAINROOM, NOP_MAINROOM, CUDDLES} collision_type_mainRoom_t;

void mainRoomController_load_mainRoom();

bool mainRoomController_checkCollision(Sprite* sprite, int x, int y);

void mainRoomController_toggleHotbar();

void mainRoomController_Hotbar_goRight();
void mainRoomController_Hotbar_goLeft();

collision_type_mainRoom_t mainRoomController_getButtonEvent();

void mainRoomController_setButtonEvent(collision_type_mainRoom_t ct);

void mainRoomController_step();

void mainRoomController_delete_mainRoom();

Cursor* getMainRoomCursor();

void setMainRoomCursor(Cursor* cursor);
