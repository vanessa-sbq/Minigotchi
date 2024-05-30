#ifndef __GUIDRAWER_H
#define __GUIDRAWER_H

#include "../device_controllers/video/video.h"

#include <lcom/lcf.h>

typedef struct {
    uint16_t height, width;
    uint32_t *colors;
} Sprite;

Sprite* guiDrawer_get_button_sprite(int text_index);
Sprite* get_cursor_sprite(); 
Sprite* guiDrawer_get_start_button_sprite(); 
Sprite* guiDrawer_get_quit_button_sprite();
Sprite* guiDrawer_get_minigotchi_sprite();
Sprite* guiDrawer_get_minigotchi_cuddle_sprite();
Sprite* guiDrawer_get_minigotchi_normal_sprite();
Sprite* guiDrawer_get_hotbar_sprite();
Sprite* guiDrawer_get_hotbar_select_sprite();
Sprite* guiDrawer_get_bar_sprite(int bar_type, int level);

Sprite* get_text_sprite_from_index(uint8_t index);
void setup_text_sprite();
void cleanup_text_sprite();

// Buffer getters
char** guiDrawer_get_backgroundBuffer();

// Buffer setters
void guiDrawer_set_backgroundBuffer(char** backgroundBuffer);

/* int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);*/
Sprite *create_sprite_xpm(xpm_map_t sprite);
void setup_sprites();
int drawSprite(Sprite *sprite, int x, int y);

// Sprite getters
uint16_t sprite_get_width(Sprite* sprite);
uint16_t sprite_get_height(Sprite* sprite);

void switchBackground(uint8_t bg);

// Wrappers
void wrapper_draw_background(/**/);

#endif /* __GUIDRAWER_H */
