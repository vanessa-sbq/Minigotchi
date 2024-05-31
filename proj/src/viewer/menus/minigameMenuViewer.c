#include "minigameMenuViewer.h"

static MinigameMenu *minigameMenu;

void minigameMenuViewer_setMinigameMenu(MinigameMenu* mm){
    minigameMenu = mm;
}

/**
 * @brief Calls draw function of each element the MainMenu contains
 */
void minigameMenuViewer_draw(){
    vg_clear_screen();

    // Background elements
    wrapper_draw_background();

    // Foreground elements
    draw_button(minigameMenu_get_tttButton(minigameMenu));
    draw_button(minigameMenu_get_rpsButton(minigameMenu));
    draw_button(minigameMenu_get_quitButton(minigameMenu));
    draw_cursor(minigameMenu_get_cursor(minigameMenu));

    vg_page_flip();
}
