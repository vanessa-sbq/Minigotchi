#include "mainRoomController.h"

static MainRoom *mainRoom = NULL;
static collision_type_mainRoom_t currentButtonEvent = NOP_MAINROOM;

void mainRoomController_load_mainRoom(){
    if (mainRoom == NULL){
        mainRoom = new_mainRoom();
        mainRoomViewer_setMainRoom(mainRoom);
    }
}

collision_type_mainRoom_t mainRoomController_checkCollision(Button* button) {
    Cursor* cursor = mainRoom_get_cursor(mainRoom);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

    int button_width = sprite_get_width(button->sprite);
    int button_height = sprite_get_height(button->sprite);

    int left_upper_corner_x = button_get_x(button);

    int right_upper_corner_x = left_upper_corner_x + button_width;

    int right_upper_corner_y = button_get_y(button);

    int left_lower_corner_y = right_upper_corner_y + button_height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
            return currentButtonEvent;
    }

    return NOP_MAINROOM;
}

collision_type_mainRoom_t mainRoomController_getButtonEvent() {
    return currentButtonEvent;
}


void mainRoomController_step(){
    mainRoomController_load_mainRoom();

    // TODO: Check button collisions :)
}

void mainRoomController_delete_mainRoom(){
    delete_mainRoom(mainRoom);
}

Cursor* getMainRoomCursor() {
    return mainRoom->cursor;
}

void setMainRoomCursor(Cursor* cursor) {
    mainRoom->cursor = cursor;
}
