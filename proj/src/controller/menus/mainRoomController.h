#include <lcom/lcf.h>

#include "../../model/stateModels/mainRoom.h"
#include "../../model/button.h"
#include "../../model/cursor.h"

#include "../../viewer/menus/mainRoomViewer.h"

typedef enum {BACK_TO_MENU, NOP} collision_type_t;

void mainRoomController_load_mainRoom();

collision_type_t mainRoomController_checkCollision(Button* mainMenu);

collision_type_t mainRoomController_getButtonEvent();

void mainRoomController_step();

void mainRoomController_destroy_mainMenu();
