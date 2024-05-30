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
#include "../sprites/xpms/happiness_bar_max.xpm"
#include "../sprites/xpms/hunger_bar_max.xpm"
#include "../sprites/xpms/nameMinigotchi_bg.xpm"
#include "../sprites/xpms/day_window.xpm"
#include "../sprites/xpms/evening_window.xpm"
#include "../sprites/xpms/night_window.xpm"

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
#include "../sprites/xpm_fonts/N.xpm"
#include "../sprites/xpm_fonts/O.xpm"
#include "../sprites/xpm_fonts/P.xpm"
#include "../sprites/xpm_fonts/Q.xpm"
#include "../sprites/xpm_fonts/R.xpm"
#include "../sprites/xpm_fonts/S.xpm"
#include "../sprites/xpm_fonts/T.xpm"
#include "../sprites/xpm_fonts/U.xpm"
#include "../sprites/xpm_fonts/V.xpm"
#include "../sprites/xpm_fonts/W.xpm"
#include "../sprites/xpm_fonts/X.xpm"
#include "../sprites/xpm_fonts/Y.xpm"
#include "../sprites/xpm_fonts/Z.xpm"

// Backgrounds
static Sprite* _bg_mainmenu_sprite = NULL;
static Sprite* _bg_mainroom_sprite = NULL;
static Sprite* _bg_minigames_sprite = NULL;
static Sprite* _bg_nameMinigotchi_sprite = NULL;

// Other Game Elements
static Sprite* _cursor_sprite = NULL;
static Sprite* _start_button_sprite = NULL;
static Sprite* _quit_button_sprite = NULL;
static Sprite* _minigotchi_sprite = NULL;
static Sprite* _minigotchi_happy_sprite = NULL;
static Sprite* _day_window = NULL;
static Sprite* _evening_window = NULL;
static Sprite* _night_window = NULL;

// GUI Elements
static Sprite* _hotbar_sprite = NULL;
static Sprite* _hotbar_select_sprite = NULL;
static Sprite* _happiness_bar_sprite = NULL;
static Sprite* _hunger_bar_sprite = NULL;
	
/*** Buffers ***/
static char* _backgroundBuffer;

/* Text */

static Sprite* _A_sprite;
static Sprite* _B_sprite;
static Sprite* _C_sprite;
static Sprite* _D_sprite;
static Sprite* _E_sprite;
static Sprite* _F_sprite;
static Sprite* _G_sprite;
static Sprite* _H_sprite;
static Sprite* _I_sprite;
static Sprite* _J_sprite;
static Sprite* _K_sprite;
static Sprite* _L_sprite;
static Sprite* _M_sprite;
static Sprite* _N_sprite;
static Sprite* _O_sprite;
static Sprite* _P_sprite;
static Sprite* _Q_sprite;
static Sprite* _R_sprite;
static Sprite* _S_sprite;
static Sprite* _T_sprite;
static Sprite* _U_sprite;
static Sprite* _V_sprite;
static Sprite* _W_sprite;
static Sprite* _X_sprite;
static Sprite* _Y_sprite;
static Sprite* _Z_sprite;


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

Sprite* guiDrawer_get_day_window() {
	return _day_window;
}

Sprite* guiDrawer_get_evening_window() {
	return _evening_window;
}

Sprite* guiDrawer_get_night_window() {
	return _night_window;
}

Sprite* guiDrawer_get_bar_sprite(int bar_type, int level){
	if (bar_type == 0){
		return _happiness_bar_sprite;
	} else {
		return _hunger_bar_sprite;
	}
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

Sprite* get_text_sprite_from_index(uint8_t index) {
	switch (index) {
		case 0:
			return _A_sprite;
			break;
		case 1:
			return _B_sprite;
			break;
		case 2:
			return _C_sprite;
			break;
		case 3:
			return _D_sprite;
			break;
		case 4:
			return _E_sprite;
			break;
		case 5:
			return _F_sprite;
			break;
		case 6:
			return _G_sprite;
			break;
		case 7:
			return _H_sprite;
			break;
		case 8:
			return _I_sprite;
			break;
		case 9:
			return _J_sprite;
			break;
		case 10:
			return _K_sprite;
			break;
		case 11:
			return _L_sprite;
			break;
		case 12:
			return _M_sprite;
			break;
		case 13:
			return _N_sprite;
			break;
		case 14:
			return _O_sprite;
			break;
		case 15:
			return _P_sprite;
			break;
		case 16:
			return _Q_sprite;
			break;
		case 17:
			return _R_sprite;
			break;
		case 18:
			return _S_sprite;
			break;
		case 19:
			return _T_sprite;
			break;
		case 20:
			return _U_sprite;
			break;
		case 21:
			return _V_sprite;
			break;
		case 22:
			return _W_sprite;
			break;
		case 23:
			return _X_sprite;
			break;
		case 24:
			return _Y_sprite;
			break;
		case 25:
			return _Z_sprite;
			break;
	}
	return NULL;
}

void setup_text_sprite() {
	_A_sprite = create_sprite_xpm((xpm_map_t) A_xpm );
	_B_sprite = create_sprite_xpm((xpm_map_t) B_xpm );
	_C_sprite = create_sprite_xpm((xpm_map_t) C_xpm );
	_D_sprite = create_sprite_xpm((xpm_map_t) D_xpm );
	_E_sprite = create_sprite_xpm((xpm_map_t) E_xpm );
	_F_sprite = create_sprite_xpm((xpm_map_t) F_xpm );
	_G_sprite = create_sprite_xpm((xpm_map_t) G_xpm );
	_H_sprite = create_sprite_xpm((xpm_map_t) H_xpm );
	_I_sprite = create_sprite_xpm((xpm_map_t) I_xpm );
	_J_sprite = create_sprite_xpm((xpm_map_t) J_xpm );
	_K_sprite = create_sprite_xpm((xpm_map_t) K_xpm );
	_L_sprite = create_sprite_xpm((xpm_map_t) L_xpm );
	_M_sprite = create_sprite_xpm((xpm_map_t) M_xpm );
	_N_sprite = create_sprite_xpm((xpm_map_t) N_xpm );
	_O_sprite = create_sprite_xpm((xpm_map_t) O_xpm );
	_P_sprite = create_sprite_xpm((xpm_map_t) P_xpm );
	_Q_sprite = create_sprite_xpm((xpm_map_t) Q_xpm );
	_R_sprite = create_sprite_xpm((xpm_map_t) R_xpm );
	_S_sprite = create_sprite_xpm((xpm_map_t) S_xpm );
	_T_sprite = create_sprite_xpm((xpm_map_t) T_xpm );
	_U_sprite = create_sprite_xpm((xpm_map_t) U_xpm );
	_V_sprite = create_sprite_xpm((xpm_map_t) V_xpm );
	_W_sprite = create_sprite_xpm((xpm_map_t) W_xpm );
	_X_sprite = create_sprite_xpm((xpm_map_t) X_xpm );
	_Y_sprite = create_sprite_xpm((xpm_map_t) Y_xpm );
	_Z_sprite = create_sprite_xpm((xpm_map_t) Z_xpm );
}

void cleanup_text_sprite() {
	free(_A_sprite);
	free(_B_sprite);
	free(_C_sprite);
	free(_D_sprite);
	free(_E_sprite);
	free(_F_sprite);
	free(_G_sprite);
	free(_H_sprite);
	free(_I_sprite);
	free(_J_sprite);
	free(_K_sprite);
	free(_L_sprite);
	free(_M_sprite);
	free(_N_sprite);
	free(_O_sprite);
	free(_P_sprite);
	free(_Q_sprite);
	free(_R_sprite);
	free(_S_sprite);
	free(_T_sprite);
	free(_U_sprite);
	free(_V_sprite);
	free(_W_sprite);
	free(_X_sprite);
	free(_Y_sprite);
	free(_Z_sprite);
}

void setup_sprites(){
	// Backgrounds
	_bg_mainmenu_sprite = create_sprite_xpm((xpm_map_t) bricks_xpm);
	_bg_mainroom_sprite = create_sprite_xpm((xpm_map_t) bg_mainroom_xpm);
	_bg_minigames_sprite = create_sprite_xpm((xpm_map_t) bricks_xpm);
	_bg_nameMinigotchi_sprite = create_sprite_xpm((xpm_map_t) nameMinigotchi_bg_xpm);
	_day_window = create_sprite_xpm((xpm_map_t) day_window_xpm);
	_evening_window = create_sprite_xpm((xpm_map_t) evening_window_xpm);
	_night_window = create_sprite_xpm((xpm_map_t) night_window_xpm);
	_backgroundBuffer = malloc(video_get_vram_size());

	// Cursor
	_cursor_sprite = create_sprite_xpm((xpm_map_t) feather_xpm);

	// Buttons
	_start_button_sprite = create_sprite_xpm((xpm_map_t) start_button_xpm);
	_quit_button_sprite = create_sprite_xpm((xpm_map_t) quit_button_xpm);

	// Bars
	_happiness_bar_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_max_xpm);
	_hunger_bar_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_max_xpm);
	
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
		case 3: // NameMinigotchi
			getBufferFromSprite(_bg_nameMinigotchi_sprite->height, _bg_nameMinigotchi_sprite->width, 0, 0, _bg_nameMinigotchi_sprite->colors, &_backgroundBuffer);
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

