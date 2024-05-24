#include <lcom/lcf.h>

#include "../../model/stateModels/mainRoom.h"
#include "../../model/button.h"
#include "../../model/cursor.h"

#include "../../viewer/menus/mainRoomViewer.h"

typedef enum {QUIT_MAINROOM, MINIGAMES, NOP_MAINROOM} collision_type_mainRoom_t;

void mainRoomController_load_mainRoom();

collision_type_mainRoom_t mainRoomController_checkCollision(Button* button);

collision_type_mainRoom_t mainRoomController_getButtonEvent();

void mainRoomController_step();

void mainRoomController_delete_mainRoom();

Cursor* getMainRoomCursor();

void setMainRoomCursor(Cursor* cursor);
