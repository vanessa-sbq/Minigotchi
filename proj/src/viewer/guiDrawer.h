#ifndef __GUIDRAWER_H
#define __GUIDRAWER_H

#include "../device_controllers/video/video.h"

#include <lcom/lcf.h>

typedef struct {
    uint16_t height, width;
    uint32_t *colors;
} Sprite;

Sprite* getSprite();
/* int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);*/
Sprite *create_sprite_xpm(xpm_map_t sprite);
void setup_sprites(char** sprite);
int drawSprite(Sprite *sprite, int x, int y);

#endif /* __GUIDRAWER_H */
