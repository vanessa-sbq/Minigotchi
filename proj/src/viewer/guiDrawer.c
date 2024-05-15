#include "guiDrawer.h"

/*** Sprites ***/
// Minigotchi
#include "../sprites/xpms/bonecos_lcom.xpm"
#include "../sprites/xpms/braco_direito.xpm"
#include "../sprites/xpms/braco_esquerdo.xpm"
#include "../sprites/xpms/Untitled.xpm"
// Font
#include "../sprites/fonts/A.xpm"


static Sprite* _sprite;
static Sprite* _cursor_sprite;

Sprite* getSprite() {
    return _sprite;
}

Sprite* get_cursor_sprite() { // TODO: temp
    return _cursor_sprite;
}

/* int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
	xpm_image_t img;
	uint8_t *color = xpm_load(xpm, XPM_8_8_8_8, &img);

	for(int i = y; i < img.height + y; i++){
		for(int j = x; j < img.width + x; j++){
			drawPixel(j, i, *color);
			color++;
		}
	}
	return 0;
}
 */
Sprite *create_sprite_xpm(xpm_map_t sprite){
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if(sp == NULL){
		panic("Error allocating memory for sprite\n");
		return NULL;
	}

	xpm_image_t img;
	sp->colors = (uint32_t *)xpm_load(sprite, XPM_8_8_8_8, &img);
	sp->height = img.height;
	sp->width = img.width;

	if(sp->colors == NULL){
		panic("Error loading sprite\n");
		free(sp);
		return NULL;
	}
	return sp;
}

//void setup_sprites(char* sprite[]){
void setup_sprites(){
	_sprite = create_sprite_xpm((xpm_map_t) braco_direito_xpm);
	_cursor_sprite = create_sprite_xpm((xpm_map_t) A_xpm);
}

int drawSprite(Sprite *sprite, int x, int y){
	uint16_t height = sprite->height;
	uint16_t width = sprite->width;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(drawPixel(j + x, i + y, sprite->colors[j + i * width]) != 0){
				return 1;
			}
		}
	}
	return 0;
}

