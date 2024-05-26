#include "hotbar.h"

Hotbar *new_hotbar(uint16_t x, uint16_t y, uint8_t highlighted) {

    Hotbar *hotbar = malloc(sizeof(*hotbar));
    if (hotbar == NULL) return NULL;

    hotbar->sprite = guiDrawer_get_hotbar_sprite();
    hotbar->x = x;
    hotbar->y = y;
    hotbar->highlighted = highlighted;
    hotbar->hidden = true;

    return hotbar;
}
uint8_t hotbar_get_highlighted(Hotbar *hotbar) {
    return hotbar->highlighted;
}

Item* hotbar_get_highlighted_item(Hotbar *hotbar) {
    return hotbar->items[hotbar->highlighted];
}

void drawHotbar(Hotbar *hotbar) {
    drawSprite(hotbar->sprite, hotbar->x, hotbar->y);
}

void hotbar_set_highlighted(Hotbar *hotbar, uint8_t highlighted) {
    hotbar->highlighted = highlighted;
}

void draw_slider(Hotbar *hotbar) {
    drawSprite(guiDrawer_get_hotbar_select_sprite(), (hotbar->x + HIGHLIGHT_HOTBAR_CELL_SIZE * hotbar->highlighted), hotbar->y);
}
