#include "minigotchi.h"

Minigotchi *new_minigotchi(int x, int y, uint32_t hunger, uint32_t energy, char* name) {
    Minigotchi *minigotchi = malloc(sizeof(*minigotchi));
    if (minigotchi == NULL) return NULL;

    minigotchi->sprite = guiDrawer_get_minigotchi_sprite();
    minigotchi->x = x;
    minigotchi->y = y; 
    minigotchi->hunger = hunger;
    minigotchi->energy = energy;
    minigotchi->name = name;
    return minigotchi;
}

void kill_minigotchi(Minigotchi *minigotchi) {
    free(minigotchi);
    // TODO: Implement a function that let's us save the information about minigotchi
}

void draw_minigotchi(Minigotchi *minigotchi) {
    drawSprite(minigotchi->sprite, minigotchi->x, minigotchi->y); // FIXME: Need to add a way to make this constsnt ?
}

// Getters and setters
Sprite* minigotchi_get_sprite(Minigotchi *minigotchi) {
    return minigotchi->sprite;
}

int minigotchi_get_x(Minigotchi *minigotchi) {
    return minigotchi->x;
}

int minigotchi_get_y(Minigotchi *minigotchi) {
    return minigotchi->y;
}


void minigotchi_set_x(Minigotchi *minigotchi, int x) {
    minigotchi->x = x;
}

void minigotchi_set_y(Minigotchi *minigotchi, int y) {
    minigotchi->y = y;
}

uint32_t minigotchi_get_energy(Minigotchi *minigotchi) {
    return minigotchi->energy;
}

uint32_t minigotchi_get_hunger(Minigotchi *minigotchi) {
    return minigotchi->hunger;
}

void minigotchi_set_energy(Minigotchi *minigotchi, uint32_t value){
    minigotchi->energy = value;
}

void minigotchi_set_hunger(Minigotchi *minigotchi, uint32_t value){
    minigotchi->hunger = value;
}

char* minigotchi_get_name(Minigotchi *minigotchi) {
    return minigotchi->name;
}

void minigotchi_set_name(Minigotchi *minigotchi, char* name) {
    minigotchi->name = name;
}

