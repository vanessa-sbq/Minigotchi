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

    if (!mainRoom_get_hotbar(mainRoom)->hidden) {
        drawHotbar(mainRoom_get_hotbar(mainRoom));
        draw_slider(mainRoom_get_hotbar(mainRoom));

        // TODO remove this
        /* mainRoom_get_hotbar(mainRoom)->highlighted++;   
        mainRoom_get_hotbar(mainRoom)->highlighted %= 9;   */ 
    }

    draw_cursor(mainRoom_get_cursor(mainRoom));



    vg_page_flip();
}

