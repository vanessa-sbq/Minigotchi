#include "mainRoom.h"

MainRoom *new_mainRoom(){
    // Allocate MainRoom object
    MainRoom *mainRoom = malloc(sizeof(*mainRoom));
    if (mainRoom == NULL) return NULL;

    // Initialize MainRoom
    mainRoom->minigotchi = new_minigotchi(467, 400, 100, 100, "John Doe");
    mainRoom->miniGamesButton = new_button(0, 0, 1);
    mainRoom->quitButton = new_button(100, 400, 1);
    mainRoom->cursor = new_cursor(400, 600); 
    mainRoom->hotbar = new_hotbar(213, 750, 0);
    return mainRoom;
}

void delete_mainRoom(MainRoom *mainRoom){
    kill_minigotchi(mainRoom->minigotchi);
    button_delete(mainRoom->miniGamesButton);
    button_delete(mainRoom->quitButton);
    free(mainRoom);
}

Minigotchi* mainRoom_get_minigotchi(MainRoom *mainRoom) {
    return mainRoom->minigotchi;
}


Button* mainRoom_get_miniGamesButton(MainRoom *mainRoom){
    return mainRoom->miniGamesButton;
}

Button* mainRoom_get_quitButton(MainRoom *mainRoom){
    return mainRoom->quitButton;
}

Cursor* mainRoom_get_cursor(MainRoom *mainRoom){
    return mainRoom->cursor;
}

Hotbar* mainRoom_get_hotbar(MainRoom *mainRoom) {
    return mainRoom->hotbar;
}


