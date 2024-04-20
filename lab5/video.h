#ifndef __VIDEO_H
#define __VIDEO_H

#include <stdbool.h>
#include <stdint.h>

int video_get_mode_information(uint16_t mode);

int video_set_graphics_mode(uint16_t mode); 	// Configure video card for desired graphics mode

int video_map_vram(); // Map memory (Map VRAM in process's address space)

unsigned int video_get_phys_addr();

unsigned int video_get_vram_size();

int drawPixel(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

int vg_draw_pixmap(uint16_t x, uint16_t y, uint8_t* map, xpm_image_t pixmap_and_metadata);

void vg_clear_screen();


#endif /* __VIDEO_H */
