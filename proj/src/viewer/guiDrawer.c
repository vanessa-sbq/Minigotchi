#include "guiDrawer.h"

/*** Sprites ***/
// Minigotchi
#include "../sprites/xpms/bonecos_lcom.xpm"
#include "../sprites/xpms/braco_direito.xpm"
#include "../sprites/xpms/braco_esquerdo.xpm"
#include "../sprites/xpms/Untitled.xpm"
#include "../sprites/xpms/start_button-xpm.xpm"
#include "../sprites/xpms/quit_button-xpm.xpm"
#include "../sprites/xpms/bricks.xpm"
#include "../sprites/xpms/bg_mainroom.xpm"
#include "../sprites/xpms/minigotchi_normal.xpm"
#include "../sprites/xpms/minigotchi_happy.xpm"

#include "../sprites/xpms/feather.xpm"

// Font
#include "../sprites/fonts/A.xpm"

static Sprite* _sprite;
static Sprite* _cursor_sprite;
static Sprite* _start_button_sprite;
static Sprite* _quit_button_sprite;
static Sprite* _background_sprite;
static Sprite* _bg_mainroom_sprite;
static Sprite* _minigotchi_sprite;
static Sprite* _minigotchi_happy_sprite;
	
static char* _backgroundBuffer;
/* 
static char* _startButtonBuffer;
static char* _quitButtonBufffer; */


/* Sprite Getters */
Sprite* getSprite() {
    return _sprite;
}
Sprite* get_cursor_sprite() {
    return _cursor_sprite;
}
Sprite* guiDrawer_get_button_sprite(int text_index){
	switch (text_index){
		case 0:
			return guiDrawer_get_start_button_sprite();
			break;
		case 1: 
			return guiDrawer_get_quit_button_sprite();
			break; 
		default:break;
	}
	return NULL;
}
Sprite* guiDrawer_get_start_button_sprite(){
	return _start_button_sprite;
}
Sprite* guiDrawer_get_quit_button_sprite(){
	return _quit_button_sprite;
}

Sprite* guiDrawer_get_minigotchi_sprite() {
	return _minigotchi_sprite;
}

Sprite* guiDrawer_get_minigotchi_cuddle_sprite(){
	return _minigotchi_happy_sprite;
}

Sprite* guiDrawer_get_minigotchi_normal_sprite() {
	return _minigotchi_sprite;
}


/* Buffer Getters */
char** guiDrawer_get_backgroundBuffer(){
	return &_backgroundBuffer;
}



/* Buffer Setters */
/* void guiDrawer_set_backgroundBuffer(char** backgroundBuffer){
	_backgroundBuffer = backgroundBuffer;
} */



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

void setup_sprites(){
	_sprite = create_sprite_xpm((xpm_map_t) braco_direito_xpm);
	_cursor_sprite = create_sprite_xpm((xpm_map_t) feather_xpm);
	_start_button_sprite = create_sprite_xpm((xpm_map_t) start_button_xpm);
	_quit_button_sprite = create_sprite_xpm((xpm_map_t) quit_button_xpm);
	_background_sprite = create_sprite_xpm((xpm_map_t) bricks_xpm);
	_bg_mainroom_sprite = create_sprite_xpm((xpm_map_t) bg_mainroom_xpm);
	_minigotchi_sprite = create_sprite_xpm((xpm_map_t) minigotchi_normal_xpm);
	_minigotchi_happy_sprite = create_sprite_xpm((xpm_map_t) minigotchi_happy_xpm);


	_backgroundBuffer = malloc(video_get_vram_size()); // FIXME: ?
	/* _startButtonBuffer = malloc(video_get_vram_size());
	_quitButtonBufffer = malloc(video_get_vram_size()); */

	getBufferFromSprite(_bg_mainroom_sprite->height, _bg_mainroom_sprite->width, 0, 0, _bg_mainroom_sprite->colors, &_backgroundBuffer);
	/* getBufferFromSprite(_start_button_sprite->height, _background_sprite->width, 0, 0, _background_sprite->colors, &_startButtonBuffer); */
	/* getBufferFromSprite(_quit_button_sprite->height, _background_sprite->width, 0, 0, _background_sprite->colors, &_quitButtonBufffer); */

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

uint16_t sprite_get_width(Sprite* sprite){
	return sprite->width;
}

uint16_t sprite_get_height(Sprite* sprite){
	return sprite->height;
}


/** Wrappers **/
void wrapper_draw_background(/**/) {
	setBackgroundFromBuffer(_backgroundBuffer); // TODO: add more with an enum;
}

