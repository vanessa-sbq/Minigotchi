#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

// TODO: fix
#include "viewer/guiDrawer.h"
#include "device_controllers/video/video.h"
#include "device_controllers/kbc/keyboard.h"
#include "device_controllers/kbc/kbc.h"
#include "model/cursor.h"

// TODO: Might need to add/remove some states
typedef enum {MAIN_MENU, MAIN_ROOM, MINIGAME_1, MINIGAME_2, EXIT} state_t;
//static state_t game_state = MAIN_MENU; // Game's current state

int frames = 60; // TODO: Make value not hard coded

int main(int argc, char *argv[]) {
	lcf_set_language("EN-US");
	lcf_trace_calls("/home/lcom/labs/proj/trace.txt");  
	lcf_log_output("/home/lcom/labs/proj/output.txt"); 
	if (lcf_start(argc, argv)) return 1;
	lcf_cleanup();
	return 0;
}


int (proj_main_loop)(int argc, char **argv) {
	// Get mode information
	if (video_get_mode_information(0x14C) != 0){
		printf("Error in video_get_mode_information()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

	// Write to VRAM to display on the screen what is requested
	if (video_map_vram() != 0){
		printf("Error in video_write_to_vram()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

    // Configure video card for desired graphics mode
	if (video_set_graphics_mode(0x14C) != 0){
		printf("Error in video_set_graphics_mode()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

	setup_sprites();
	Cursor *cursor = new_cursor(10, 10);


	// TODO: Temp (to clear memory)
	/* vg_clear_screen();
	vg_page_flip();
	vg_clear_screen(); */

	// Game Loop
	while (true){ // TODO: Change condition
		tickdelay(micros_to_ticks(50000)); // Needed in order to prevent flickering (FIXME:)
		vg_clear_screen();
		vg_draw_rectangle(0, 0, 1000, 1000, 0xFF123);
		if(drawSprite(cursor->sprite, cursor->x, cursor->y) != 0){
			printf("Error in drawSprite()\n");
			return 1;
		}
		vg_page_flip();
	}
	

	delete_cursor(cursor);

    uint8_t bit_no = 0;
    if (subscribe_interrupts_kbd(&bit_no) != 0){
        return 1;
    }

	if (unsubscribe_interrupts_kbd() != 0){
        return 1;
    }

    // Return to text mode.
    if (vg_exit() != 0){
		printf("Error in vg_exit()\n");
		return 1;
	}

    return 0;
}


