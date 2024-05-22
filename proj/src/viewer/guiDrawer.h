#ifndef __GUIDRAWER_H
#define __GUIDRAWER_H

#include "../device_controllers/video/video.h"

#include <lcom/lcf.h>

typedef struct {
    uint16_t height, width;
    uint32_t *colors;
} Sprite;

Sprite* getSprite();
Sprite* get_cursor_sprite(); 
Sprite* get_button_sprite();
/* int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);*/
Sprite *create_sprite_xpm(xpm_map_t sprite);
void setup_sprites();
int drawSprite(Sprite *sprite, int x, int y);

// Sprite getters
uint16_t sprite_get_width(Sprite* sprite);
uint16_t sprite_get_height(Sprite* sprite);

#endif /* __GUIDRAWER_H */
