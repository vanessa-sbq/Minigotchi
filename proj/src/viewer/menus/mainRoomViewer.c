#include "mainRoomViewer.h"

static MainRoom *mainRoom;
static Sprite* currentWindow;

void setRTCWindow(Sprite* window) {
    currentWindow = window;
}

void mainRoomViewer_setMainRoom(MainRoom* mr){
    mainRoom = mr;
}

/**
 * @brief Calls draw function of each element the main room contains
 */
void mainRoomViewer_draw(){
    vg_clear_screen();

    // Background elements
    if (!mainRoom_get_hotbar(mainRoom)->hidden) {
        wrapper_draw_background(1);
    } else {
        wrapper_draw_background(0);
    }
    
    drawSprite(currentWindow, 879, 285);
    draw_minigotchi(mainRoom_get_minigotchi(mainRoom));

    // Foreground elements
    draw_button(mainRoom_get_miniGamesButton(mainRoom));
    draw_button(mainRoom_get_quitButton(mainRoom));
    draw_bar(mainRoom_get_happinessBar(mainRoom));
    draw_bar(mainRoom_get_hungerBar(mainRoom));

    if (!mainRoom_get_hotbar(mainRoom)->hidden) {
        drawHotbar(mainRoom_get_hotbar(mainRoom));
        draw_slider(mainRoom_get_hotbar(mainRoom));
    }

    draw_cursor(mainRoom_get_cursor(mainRoom));

    vg_page_flip();
}

