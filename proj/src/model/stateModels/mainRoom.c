#include "mainRoom.h"

MainRoom *new_mainRoom(){
    // Allocate MainRoom object
    MainRoom *mainRoom = malloc(sizeof(*mainRoom));
    if (mainRoom == NULL) return NULL;

    // Initialize MainRoom
    mainMenu->startNewButton = new_button(100, 0, 0); 
    mainMenu->continueButton = new_button(100, 200, 0); 
    mainMenu->quitButton = new_button(100, 400, 1);
    mainMenu->cursor = new_cursor(400, 600); 
    return mainMenu;
}

void delete_mainRoom(MainMenu *mainRoom){
    free(mainRoom); // FIXME: Should free the attributes too
}

void draw_mainRoom(MainMenu *mainMenu){
    draw_button(mainMenu->startNewButton);
    draw_button(mainMenu->continueButton);
    draw_button(mainMenu->quitButton);
    draw_cursor(mainMenu->cursor);
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


