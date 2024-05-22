#include <lcom/lcf.h>
#include "video_macros.h"

void returnToTextMode();
int video_get_mode_information(uint16_t mode);
int video_map_vram();
int video_set_graphics_mode(uint16_t mode);
unsigned int video_get_phys_addr();
unsigned int video_get_vram_size();

int drawPixel(uint16_t x, uint16_t y, uint32_t color);
int vg_draw_pixmap(uint16_t x, uint16_t y, uint8_t* map, xpm_image_t pixmap_and_metadata);
void vg_clear_screen();
int vg_page_flip();
