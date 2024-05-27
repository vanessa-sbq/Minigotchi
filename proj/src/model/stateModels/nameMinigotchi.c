#include "nameMinigotchi.h"

NameMinigotchi *new_nameMinigotchi() {
    NameMinigotchi *nameMinigotchi = malloc(sizeof(*nameMinigotchi));
    if (nameMinigotchi == NULL) return NULL;

    nameMinigotchi->continueButton = new_button(448, 400, 0);
    nameMinigotchi->quitButton = new_button(448, 600, 1);
    nameMinigotchi->cursor = new_cursor(400, 600);
    return nameMinigotchi;
}

void delete_nameMinigotchi(NameMinigotchi *nameMinigotchi) {
    free(nameMinigotchi);
}

void draw_nameMinigotchi(NameMinigotchi *nameMinigotchi);

Button* nameMinigotchi_get_continueButton(NameMinigotchi *nameMinigotchi) {
    return nameMinigotchi->continueButton;
}

Button* nameMinigotchi_get_quitButton(NameMinigotchi *nameMinigotchi) {
    return nameMinigotchi->quitButton;
}

Cursor* nameMinigotchi_get_cursor(NameMinigotchi *nameMinigotchi) {
    return nameMinigotchi->cursor;
}


void nameMinigotchi_setName(NameMinigotchi *nameMinigotchi, char* nm) {
    nameMinigotchi->name = nm;
}

char* nameMinigotchi_getName(NameMinigotchi* nameMinigotchi) {
    return nameMinigotchi->name;
}

