#include "mainRoomViewer.h"

static MainRoom *mainRoom;

void mainRoomViewer_setMainRoom(MainRoom* mr){
    mainRoom = mr;
}

//static int x = 10;

/**
 * @brief Calls draw function of each element the MainMenu contains
 */
void mainRoomViewer_draw(){
    vg_clear_screen();

    // Background elements
    wrapper_draw_background();
    draw_minigotchi(mainRoom_get_minigotchi(mainRoom)); // TODO: add viewer in case of animations.

    // Foreground elements
    draw_button(mainRoom_get_miniGamesButton(mainRoom));
    draw_button(mainRoom_get_quitButton(mainRoom));
    draw_cursor(mainRoom_get_cursor(mainRoom));

    vg_page_flip();
}

