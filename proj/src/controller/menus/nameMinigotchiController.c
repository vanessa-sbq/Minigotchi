#include "nameMinigotchiController.h"


static NameMinigotchi *nameMinigotchi = NULL;
static collision_type_nameMinigotchi_t currentButtonEvent = NOP_NAMEMINIGOTCHI;

void nameMinigotchiController_load_nameMinigotchi() {
    if (nameMinigotchi == NULL){
        nameMinigotchi = new_nameMinigotchi();
        nameMinigotchiViewer_setNameMinigotchi(nameMinigotchi);
    }
}

bool nameMinigotchiController_checkCollision(Sprite* sprite, int x, int y) {
    Cursor* cursor = nameMinigotchi_get_cursor(nameMinigotchi);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

    int sprite_width = sprite_get_width(sprite);
    int sprite_height = sprite_get_height(sprite);

    int left_upper_corner_x = x;

    int right_upper_corner_x = left_upper_corner_x + sprite_width;

    int right_upper_corner_y = y;

    int left_lower_corner_y = right_upper_corner_y + sprite_height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
            return true;
    }

    return false;
}

collision_type_nameMinigotchi_t nameMinigotchiController_getButtonEvent() {
    return currentButtonEvent;
}

void nameMinigotchiController_setButtonEvent(collision_type_nameMinigotchi_t ct) {
    currentButtonEvent = ct;
}

void nameMinigotchiController_step() {
    nameMinigotchiController_load_nameMinigotchi();
    
    Button* continue_button = nameMinigotchi_get_continueButton(nameMinigotchi);
    if (nameMinigotchiController_checkCollision(button_get_sprite(continue_button), button_get_x(continue_button), button_get_y(continue_button))){
        currentButtonEvent = CONTINUE_NAMEMINIGOTCHI;
    }

    Button* quit_button = nameMinigotchi_get_quitButton(nameMinigotchi);
    if (nameMinigotchiController_checkCollision(button_get_sprite(quit_button), button_get_x(quit_button), button_get_y(quit_button))){
        currentButtonEvent = QUIT_NAMEMINIGOTCHI;
    }

}

Cursor* getNameMinigotchiCursor() {
    currentButtonEvent = NOP_NAMEMINIGOTCHI;
    return nameMinigotchi->cursor;
}

void setNameMinigotchiCursor(Cursor* cursor) {
    nameMinigotchi->cursor = cursor;
}

void nameMinigotchiController_delete_nameMinigotchi() {
    delete_nameMinigotchi(nameMinigotchi);
}
