#include "mainRoomController.h"

static MainRoom *mainRoom = NULL;
static collision_type_mainRoom_t currentButtonEvent = NOP_MAINROOM;
static Database* database;


void mainRoomController_load_mainRoom(){
    if (mainRoom == NULL){
        database = getDatabase();
        mainRoom = new_mainRoom();
        mainRoomViewer_setMainRoom(mainRoom);
        mainRoom->minigotchi->name = database_get_minigotchiName(database);
        
    }
}

void mainRoomController_toggleHotbar() {
    mainRoom->hotbar->hidden = !mainRoom->hotbar->hidden;
}

void mainRoomController_Hotbar_goRight() {
    mainRoom->hotbar->highlighted++;
    mainRoom->hotbar->highlighted %= 9;

    
}

void mainRoomController_Hotbar_goLeft() {
    if (mainRoom->hotbar->highlighted == 0) {
        mainRoom->hotbar->highlighted = 9;
    }
    mainRoom->hotbar->highlighted--; // 1111 1111 1111 1111
}

bool mainRoomController_checkCollision(Sprite* sprite, int x, int y) {
    Cursor* cursor = mainRoom_get_cursor(mainRoom);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

    int sprite_width = sprite_get_width(sprite);
    int sprite_height = sprite_get_height(sprite);

    int left_upper_corner_x = x;

    int right_upper_corner_x = left_upper_corner_x + sprite_width;

    int right_upper_corner_y = y;

    int left_lower_corner_y = right_upper_corner_y + sprite_height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
            return true;
    }

    return false;
}

collision_type_mainRoom_t mainRoomController_getButtonEvent() {
    return currentButtonEvent;
}

void mainRoomController_setButtonEvent(collision_type_mainRoom_t ct) {
    currentButtonEvent = ct;
}

void mainRoomController_step(){
    mainRoomController_load_mainRoom();
    
    // Check if "Quit" clicked
    Button* quit_button = mainRoom_get_quitButton(mainRoom);
    if (mainRoomController_checkCollision(button_get_sprite(quit_button), button_get_x(quit_button), button_get_y(quit_button))){
        currentButtonEvent = QUIT_MAINROOM;
    }

    // Check if "Minigames" clicked
    Button* minigames_button = mainRoom_get_miniGamesButton(mainRoom);
    if (mainRoomController_checkCollision(button_get_sprite(minigames_button), button_get_x(minigames_button), button_get_y(minigames_button))){
        currentButtonEvent = MINIGAMES_MAINROOM;
    }

    Minigotchi* minigotchi = mainRoom_get_minigotchi(mainRoom);
    if (mainRoomController_checkCollision(minigotchi_get_sprite(minigotchi), minigotchi_get_x(minigotchi), minigotchi_get_y(minigotchi))){
        minigotchi_set_sprite(minigotchi, guiDrawer_get_minigotchi_cuddle_sprite());
        minigotchi_set_cuddles(minigotchi, true);
    }else{
        minigotchi_set_sprite(minigotchi, guiDrawer_get_minigotchi_normal_sprite());
        minigotchi_set_cuddles(minigotchi, false);
    }

}

void mainRoomController_delete_mainRoom(){
    delete_mainRoom(mainRoom);
}

Cursor* getMainRoomCursor() {
    currentButtonEvent = NOP_MAINROOM;
    return mainRoom->cursor;
}

void setMainRoomCursor(Cursor* cursor) {
    mainRoom->cursor = cursor;
}
