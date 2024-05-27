#include "mainRoom.h"

MainRoom *new_mainRoom(){
    // Allocate MainRoom object
    MainRoom *mainRoom = malloc(sizeof(*mainRoom));
    if (mainRoom == NULL) return NULL;

    // Initialize MainRoom
    mainRoom->minigotchi = new_minigotchi(467, 400, 100, 100, "John Doe");
    mainRoom->miniGamesButton = new_button(10, 10, 1);
    mainRoom->quitButton = new_button(886, 10, 1);
    mainRoom->happinessBar = new_bar(10, 100, 1000, 0); // FIXME: Might need to change arguments
    mainRoom->hungerBar = new_bar(10, 150, 1000, 1); // FIXME: Might need to change arguments
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

Bar* mainRoom_get_happinessBar(MainRoom *mainRoom){
    return mainRoom->happinessBar;
}

Bar* mainRoom_get_hungerBar(MainRoom *mainRoom){
    return mainRoom->hungerBar;
}

Cursor* mainRoom_get_cursor(MainRoom *mainRoom){
    return mainRoom->cursor;
}

Hotbar* mainRoom_get_hotbar(MainRoom *mainRoom) {
    return mainRoom->hotbar;
}


