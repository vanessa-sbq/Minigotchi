#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

// TODO: fix
#include "viewer/guiDrawer.h"
#include "device_controllers/video/video.h"
#include "device_controllers/kbc/keyboard.h"
#include "device_controllers/kbc/kbc.h"


// TODO: fix
#include "sprites/xpms/bonecos_lcom.xpm"
#include "sprites/xpms/braco_direito.xpm"
#include "sprites/xpms/braco_esquerdo.xpm"

int main(int argc, char *argv[]) {
	lcf_set_language("EN-US");
	lcf_trace_calls("/home/lcom/labs/proj/trace.txt");  
	lcf_log_output("/home/lcom/labs/proj/output.txt"); 
	if (lcf_start(argc, argv)) return 1;
	lcf_cleanup();
	return 0;
}


int (proj_main_loop)(int argc, char **argv) {
    /* your main loop code here */

	int x = 0;
	int y = 0;


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

    setup_sprites(braco_esquerdo);
    if(drawSprite(getSprite(), x, y) != 0){
        printf("Error in drawSprite()\n");
        return 1;
    }

    uint8_t bit_no = 0;
    if (subscribe_interrupts_kbd(&bit_no) != 0){
        return 1;
    }

	sleep(10);

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

