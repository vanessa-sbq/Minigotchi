#include "mainMenuController.h"

static MainMenu *mainMenu = NULL;
static collision_type_t currentButton = START;

void mainMenuController_load_mainMenu(){
    if (mainMenu == NULL){
        mainMenu = new_mainMenu();
        mainMenuViewer_setMainMenu(mainMenu);
    }
}


collision_type_t mainMenuController_checkCollision(Button *button) {
    Cursor* cursor = mainMenu_get_cursor(mainMenu);

    int16_t cursorX = cursor_get_x(cursor);
    int16_t cursorY = cursor_get_y(cursor);

    

    int16_t button_width = sprite_get_width(button->sprite);
    int16_t button_height = sprite_get_height(button->sprite);

    int16_t left_upper_corner_x = button_get_x(button);

    int16_t right_upper_corner_x = left_upper_corner_x + button_width;

    int16_t right_upper_corner_y = button_get_y(button);

    int16_t left_lower_corner_y = right_upper_corner_y + button_height;

    if ( cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) {
        if (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) {
            return currentButton;
        }
    }

    return NOP;
}

void mainMenuController_step(){
    mainMenuController_load_mainMenu();


    //draw_mainMenu(mainMenu);

    // TODO: check return of checkCollision and do something about it

    currentButton = START;
    mainMenuController_checkCollision(mainMenu_get_startNewButton(mainMenu));
    currentButton = CONTINUE;
    mainMenuController_checkCollision(mainMenu_get_continueButton(mainMenu));
    currentButton = QUIT;
    mainMenuController_checkCollision(mainMenu_get_quitButton(mainMenu));

}

void mainMenuController_delete_mainMenu(){
    delete_mainMenu(mainMenu);
}

Cursor* getMainMenuCursor() {
    return mainMenu->cursor;
}


void setMainMenuCursor(Cursor* cursor) {
    mainMenu->cursor = cursor;
}
