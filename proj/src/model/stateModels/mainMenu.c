#include "mainMenu.h"

MainMenu *new_mainMenu(){
    // Allocate MainMenu object
    MainMenu *mainMenu = malloc(sizeof(*mainMenu));
    if (mainMenu == NULL) return NULL;

    // draw MAIN MENU // FIXME:

    // Initialize MainMenu
    mainMenu->startNewButton = new_button(448, 216, 0);
    mainMenu->continueButton = new_button(448, 400, 0);
    mainMenu->quitButton = new_button(448, 600, 1);
    mainMenu->cursor = new_cursor(400, 600);
    return mainMenu;
}

void delete_mainMenu(MainMenu *mainMenu){
    button_delete(mainMenu->startNewButton);
    button_delete(mainMenu->continueButton);
    button_delete(mainMenu->quitButton);
    free(mainMenu);
}

Button* mainMenu_get_startNewButton(MainMenu *mainMenu){
    return mainMenu->startNewButton;
}

Button* mainMenu_get_continueButton(MainMenu *mainMenu){
    return mainMenu->continueButton;
}

Button* mainMenu_get_quitButton(MainMenu *mainMenu){
    return mainMenu->quitButton;
}

Cursor* mainMenu_get_cursor(MainMenu *mainMenu){
    return mainMenu->cursor;
}


