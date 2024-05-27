#include "nameMinigotchiViewer.h"

static NameMinigotchi *nameMinigotchi;

void nameMinigotchiViewer_setNameMinigotchi(NameMinigotchi* nm){
    nameMinigotchi = nm;
}

/**
 * @brief Calls draw function of each element the nameMinigotchi contains
 */
void nameMinigotchiViewer_draw(){
    vg_clear_screen();

    // Background elements
    wrapper_draw_background();

    // Foreground elements
    draw_button(nameMinigotchi_get_continueButton(nameMinigotchi));
    draw_button(nameMinigotchi_get_quitButton(nameMinigotchi));
    draw_cursor(nameMinigotchi_get_cursor(nameMinigotchi));

    vg_page_flip();
}

