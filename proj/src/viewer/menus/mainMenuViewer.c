#include "mainMenuViewer.h"

static MainMenu *mainMenu;

void mainMenuViewer_setMainMenu(MainMenu* mm){
    mainMenu = mm;
}

/**
 * @brief Calls draw function of each element the MainMenu contains
 */
void mainMenuViewer_draw(){
    vg_clear_screen();
    //wrapper_draw_background();
    //vg_draw_rectangle(0, 0, 1152, 864, 0xFF123);

    //draw_button(mainMenu->startNewButton);
    //draw_button(mainMenu->continueButton);
    //draw_button(mainMenu->quitButton);
    draw_cursor(mainMenu->cursor);
    vg_page_flip();
}

