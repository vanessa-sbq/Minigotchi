#include "guiDrawer.h"

/*** Sprites ***/
// Game Elements
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
#include "../sprites/xpms/hotbar.xpm"
#include "../sprites/xpms/hotbar_select.xpm"
#include "../sprites/xpms/feather.xpm"

// Font
#include "../sprites/xpm_fonts/A.xpm"
#include "../sprites/xpm_fonts/B.xpm"
#include "../sprites/xpm_fonts/C.xpm"
#include "../sprites/xpm_fonts/D.xpm"
#include "../sprites/xpm_fonts/E.xpm"
#include "../sprites/xpm_fonts/F.xpm"
#include "../sprites/xpm_fonts/G.xpm"
#include "../sprites/xpm_fonts/H.xpm"
#include "../sprites/xpm_fonts/I.xpm"
#include "../sprites/xpm_fonts/J.xpm"
#include "../sprites/xpm_fonts/K.xpm"
#include "../sprites/xpm_fonts/L.xpm"
#include "../sprites/xpm_fonts/M.xpm"

// Backgrounds
static Sprite* _bg_mainmenu_sprite = NULL;
static Sprite* _bg_mainroom_sprite = NULL;
static Sprite* _bg_minigames_sprite = NULL;

// Other Game Elements
static Sprite* _cursor_sprite = NULL;
static Sprite* _start_button_sprite = NULL;
static Sprite* _quit_button_sprite = NULL;
static Sprite* _minigotchi_sprite = NULL;
static Sprite* _minigotchi_happy_sprite = NULL;

// GUI Elements
static Sprite* _hotbar_sprite = NULL;
static Sprite* _hotbar_select_sprite = NULL;
	
/*** Buffers ***/
static char* _backgroundBuffer;


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

Sprite* guiDrawer_get_hotbar_sprite() {
	return _hotbar_sprite;
}

Sprite* guiDrawer_get_hotbar_select_sprite() {
	return _hotbar_select_sprite;
}



/* Buffer Getters */
char** guiDrawer_get_backgroundBuffer(){
	return &_backgroundBuffer;
}

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
	// Backgrounds
	_bg_mainmenu_sprite = create_sprite_xpm((xpm_map_t) bricks_xpm);
	_bg_mainroom_sprite = create_sprite_xpm((xpm_map_t) bg_mainroom_xpm);
	_bg_minigames_sprite = create_sprite_xpm((xpm_map_t) bricks_xpm);
	_backgroundBuffer = malloc(video_get_vram_size());

	// Cursor
	_cursor_sprite = create_sprite_xpm((xpm_map_t) feather_xpm);

	// Buttons
	_start_button_sprite = create_sprite_xpm((xpm_map_t) start_button_xpm);
	_quit_button_sprite = create_sprite_xpm((xpm_map_t) quit_button_xpm);
	
	// Minigotchi
	_minigotchi_sprite = create_sprite_xpm((xpm_map_t) minigotchi_normal_xpm);
	_minigotchi_happy_sprite = create_sprite_xpm((xpm_map_t) minigotchi_happy_xpm);

	// Hotbar / hotbar items
	_hotbar_sprite = create_sprite_xpm((xpm_map_t) hotbar_xpm);
	_hotbar_select_sprite = create_sprite_xpm((xpm_map_t) hotbar_select_xpm);
}

void switchBackground(uint8_t bg) {
	switch (bg) {
		case 0: // MainMenu
			getBufferFromSprite(_bg_mainmenu_sprite->height, _bg_mainmenu_sprite->width, 0, 0, _bg_mainmenu_sprite->colors, &_backgroundBuffer);
			break;
		case 1: // MainRoom
			getBufferFromSprite(_bg_mainroom_sprite->height, _bg_mainroom_sprite->width, 0, 0, _bg_mainroom_sprite->colors, &_backgroundBuffer);
			break;
		case 2: // MinigamesMenu
			getBufferFromSprite(_bg_minigames_sprite->height, _bg_minigames_sprite->width, 0, 0, _bg_minigames_sprite->colors, &_backgroundBuffer);
			break;
		case 3:
			//getBufferFromSprite(_bg_mainroom_sprite->height, _bg_mainroom_sprite->width, 0, 0, _bg_mainroom_sprite->colors, &_backgroundBuffer);
			break;
		case 4:
			//getBufferFromSprite(_bg_mainroom_sprite->height, _bg_mainroom_sprite->width, 0, 0, _bg_mainroom_sprite->colors, &_backgroundBuffer);
			break;
		case 5:
			//getBufferFromSprite(_bg_mainroom_sprite->height, _bg_mainroom_sprite->width, 0, 0, _bg_mainroom_sprite->colors, &_backgroundBuffer);
			break;
		default:
			break;
	}
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


/*** Wrappers ***/
void wrapper_draw_background(/**/) {
	setBackgroundFromBuffer(_backgroundBuffer); // TODO: add more with an enum;
}

