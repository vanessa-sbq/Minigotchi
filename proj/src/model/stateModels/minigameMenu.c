#include "minigameMenu.h"

MinigameMenu *new_minigameMenu(){
    // Allocate MinigameMenu object
    MinigameMenu *minigameMenu = malloc(sizeof(*minigameMenu));
    if (minigameMenu == NULL) return NULL;

    // Initialize MinigameMenu
    minigameMenu->tttButton = new_button(0, 0, 1);
    minigameMenu->rpsButton = new_button(0, 200, 1);
    minigameMenu->quitButton = new_button(0, 700, 1);
    minigameMenu->cursor = new_cursor(400, 600);

    /* minigameMenu->quitButton = new_button(0, 700, 1);
    if (minigameMenu->quitButton == NULL) {
        free(minigameMenu);
        return NULL;
    }

    minigameMenu->cursor = new_cursor(400, 600); 
    if (minigameMenu->cursor == NULL) {
        button_delete(minigameMenu->quitButton);
        free(minigameMenu);
        return NULL;
    } */

    return minigameMenu;
}

void delete_minigameMenu(MinigameMenu *minigameMenu){ 
    if (minigameMenu != NULL) {
        button_delete(minigameMenu->tttButton);
        button_delete(minigameMenu->rpsButton);
        button_delete(minigameMenu->quitButton);
        delete_cursor(minigameMenu->cursor);
        free(minigameMenu);
    }
}

Button* minigameMenu_get_tttButton(MinigameMenu *minigameMenu){
    return minigameMenu->tttButton;
}

Button* minigameMenu_get_rpsButton(MinigameMenu *minigameMenu){
    return minigameMenu->rpsButton;
}

Button* minigameMenu_get_quitButton(MinigameMenu *minigameMenu){
    return minigameMenu->quitButton;
}

Cursor* minigameMenu_get_cursor(MinigameMenu *minigameMenu){
    return minigameMenu->cursor;
}
