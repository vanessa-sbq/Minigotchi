// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "keyboard.h"
#include "timer_utils.h"

#define ESC 0x81

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
	// sets the language of LCF messages (can be either EN-US or PT-PT)
	lcf_set_language("EN-US");

	// enables to log function invocations that are being "wrapped" by LCF
	// [comment this out if you don't want/need it]
	lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

	// enables to save the output of printf function calls on a file
	// [comment this out if you don't want/need it]
	lcf_log_output("/home/lcom/labs/lab5/output.txt");

	// handles control over to LCF
	// [LCF handles command line arguments and invokes the right function]
	if (lcf_start(argc, argv))
		return 1;

	// LCF clean up tasks
	// [must be the last statement before return]
	lcf_cleanup();

	return 0;
}


int(video_test_init)(uint16_t mode, uint8_t delay) {
	// Get video mode information
	if (video_get_mode_information(mode) != 0){
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
	if (video_set_graphics_mode(mode) != 0){
		printf("Error in video_set_graphics_mode()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

	// Delay
	tickdelay(micros_to_ticks(delay * 1000000));

	// Reset video card to text mode
	if (vg_exit() != 0){
		printf("Error in vg_exit()\n");
		return 1;
	}

	return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
	// Get video mode information
	if (video_get_mode_information(mode) != 0){
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
	if (video_set_graphics_mode(mode) != 0){
		printf("Error in video_set_graphics_mode()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

	vg_draw_rectangle(x, y, width, height, color);
	
	uint8_t bit_no = 0;
    if (subscribe_interrupts_kbd(&bit_no) != 0){
        return 1;
    }

    uint8_t irq_set_kbd = (0x01 << bit_no);
    int r;
    int ipc_status;
    message msg;
    bool stop = false;
    uint8_t scanCodes[2];
    uint8_t counter = 0;
    while( !stop ) { 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbc_ih();
                        
                        if (getError()){
                            setError(false);
                            continue;
                        }
                        
                        uint8_t scanCode = getScancode(); // Fetch the scancode.
                        bool breakCode = (scanCode & 0x80);

                        // check if the scan code is a break code for ESC and end program
                        if (scanCode == ESC && breakCode) {stop = true;}
                        
                        switch (counter){
                        case 0:
                            scanCodes[counter] = scanCode;
                            if (scanCode == 0xe0){
                            counter++;
                            } else {
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            }

                            break;
                        case 1:
                            scanCodes[counter] = scanCode;
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            break;
                        default:
                            return -1;
                            break;
                        }
                    }
                    break;
                default:
                    break; 
            }
        }
    }

    if (unsubscribe_interrupts_kbd() != 0){
        return 1;
    }

	// Reset video card to text mode
	if (vg_exit() != 0){
		printf("Error in vg_exit()\n");
		return 1;
	}

	return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
	if (video_get_mode_information(mode) != 0){
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
	if (video_set_graphics_mode(mode) != 0){
		printf("Error in video_set_graphics_mode()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

	if (vg_draw_pattern(no_rectangles, first, step) != 0){
		printf("Error in vg_draw_pattern()\n");
		vg_exit();
		return 1;
	}
	
	uint8_t bit_no = 0;
    if (subscribe_interrupts_kbd(&bit_no) != 0){
        return 1;
    }

    uint8_t irq_set_kbd = (0x01 << bit_no);
    int r;
    int ipc_status;
    message msg;
    bool stop = false;
    uint8_t scanCodes[2];
    uint8_t counter = 0;
    while( !stop ) { 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbc_ih();
                        
                        if (getError()){
                            setError(false);
                            continue;
                        }
                        
                        uint8_t scanCode = getScancode(); // Fetch the scancode.
                        bool breakCode = (scanCode & 0x80);

                        // check if the scan code is a break code for ESC and end program
                        if (scanCode == ESC && breakCode) {stop = true;}
                        
                        switch (counter){
                        case 0:
                            scanCodes[counter] = scanCode;
                            if (scanCode == 0xe0){
                            counter++;
                            } else {
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            }

                            break;
                        case 1:
                            scanCodes[counter] = scanCode;
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            break;
                        default:
                            return -1;
                            break;
                        }
                    }
                    break;
                default:
                    break; 
            }
        }
    }

    if (unsubscribe_interrupts_kbd() != 0){
        return 1;
    }

	// Reset video card to text mode
	if (vg_exit() != 0){
		printf("Error in vg_exit()\n");
		return 1;
	}

	return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    /* Initialize the video mode. */

    // Get video mode information
    if (video_get_mode_information(0x105) != 0){
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
	if (video_set_graphics_mode(0x105) != 0){
		printf("Error in video_set_graphics_mode()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

    enum xpm_image_type type = XPM_INDEXED;
    xpm_image_t pixmap_and_metadata;

    // Returns a pointer to an array with the colors
    uint8_t *map = xpm_load(xpm, type, &pixmap_and_metadata);

    if (map == NULL) {
        printf("Map is null.\n");
        vg_exit();
        return 1;
    }

    // Draw the pixmap
    if (vg_draw_pixmap(x, y, map, pixmap_and_metadata) != 0){
        vg_exit();
        return 1;
    }

    uint8_t bit_no = 0;
    if (subscribe_interrupts_kbd(&bit_no) != 0){
        return 1;
    }

    uint8_t irq_set_kbd = (0x01 << bit_no);
    int r;
    int ipc_status;
    message msg;
    bool stop = false;
    uint8_t scanCodes[2];
    uint8_t counter = 0;
    while( !stop ) { 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbc_ih();
                        
                        if (getError()){
                            setError(false);
                            continue;
                        }
                        
                        uint8_t scanCode = getScancode(); // Fetch the scancode.
                        bool breakCode = (scanCode & 0x80);

                        // check if the scan code is a break code for ESC and end program
                        if (scanCode == ESC && breakCode) {stop = true;}
                        
                        switch (counter){
                        case 0:
                            scanCodes[counter] = scanCode;
                            if (scanCode == 0xe0){
                            counter++;
                            } else {
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            }

                            break;
                        case 1:
                            scanCodes[counter] = scanCode;
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            break;
                        default:
                            return -1;
                            break;
                        }
                    }
                    break;
                default:
                    break; 
            }
        }
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


extern int counter;
int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {

    // Get video mode information
    if (video_get_mode_information(0x105) != 0){
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
	if (video_set_graphics_mode(0x105) != 0){
		printf("Error in video_set_graphics_mode()\n");
		if (vg_exit() != 0) printf("Error in vg_exit()\n");
		return 1;
	}

	// Subscribe Timer 0 interrupts
	uint8_t timer0_bit_no = 0;
	if (timer_subscribe_int(&timer0_bit_no) != 0){
		printf("Error in timer_subscribe_int()\n");
		return 1;
	}

	// Subscribe KBC interrupts
	uint8_t kbd_bit_no = 0;
	if (subscribe_interrupts_kbd(&kbd_bit_no) != 0){
		printf("Error in kbd_subscribe_int()\n");
		return 1;
	}


    ///////////////////////////////////////////////////////
    enum xpm_image_type type = XPM_INDEXED;
    xpm_image_t pixmap_and_metadata;

    // Returns a pointer to an array with the colors
    uint8_t *map = xpm_load(xpm, type, &pixmap_and_metadata);

    if (map == NULL) {
        printf("Map is null.\n");
        vg_exit();
        return 1;
    }

    int x_init = xi; // Store initial x position.
    int y_init = yi; // Store initial y position.
    bool moveX = true; // Either vertical (false) or horizontal movement (true).
    bool stopDrawing = false; // Store initial x position
    bool YPositiveAxis = false; // Moving along the positive x-axis.
    bool XPositiveAxis = false; // Moving along the positive y-axis.

    vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata); // Draw the first position.

    if (x_init == xf && y_init != yf){ // Case where we are suppost to move in the y-axis
        moveX = false; // We are not going to move along the x-axis.
        YPositiveAxis = (yf >= yi); // Check if we are going to move in the positive y-axis.
    }

    if (y_init == yf && x_init != xf){ // Case where we are suppost to move in the x-axis
        XPositiveAxis = (xf >= xi); // Check if we are going to move in the positive x-axis.
    }

    ///////////////////////////////////////////////////////

    uint8_t kbd_int_bit = BIT(kbd_bit_no);
	uint8_t timer0_int_bit = BIT(timer0_bit_no);
    int ipc_status;
    int r;
    message msg;
    bool stop = false;
    uint8_t scanCodes[2];
    uint8_t counter = 0;
    unsigned int frames = 0;

    while( !stop ) { 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & timer0_int_bit) {
						timer_int_handler();

                        
						if (((getCounter() - (sys_hz()/fr_rate)) == 0)){                             
                            
                            if (moveX) { // Move along the x-axis
                                if (XPositiveAxis){ // Move along the positive x-axis.

                                    /* Begin XPositiveAxis Logic */
                                    if (speed >= 1){
                                        if (x_init <= xf && !stopDrawing) { // Drawing requires checking if the current pos is already the final pos.

                                            if (x_init + speed <= xf) { // Check if the next position is valid ( less than final position )
                                                x_init += speed;
                                                vg_clear_screen();
                                                vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                                
                                            } else {
                                                vg_clear_screen();
                                                vg_draw_pixmap(xf, y_init, map, pixmap_and_metadata);
                                                stopDrawing = true;
                                            }

                                        } else {
                                            stopDrawing = true;
                                        }
                                    } else {
                                        frames++;

                                        if (frames == ((unsigned int)-speed) && !stopDrawing && x_init < (int)xf) {
                                            vg_clear_screen();
                                            x_init++;
                                            vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                            frames = 0; // Reset the frame counter.
                                        }
                                        
                                    }
                                    /* End XPositiveAxis Logic */

                                } else {

                                    /* Begin XNegativeAxis Logic */
                                    if (speed >= 1){    
                                        if (x_init >= xf && !stopDrawing) { // Drawing requires checking if the current pos is already the final pos.

                                            if (x_init - speed >= (int)xf) { // Check if the next position is valid ( less than final position )
                                                x_init -= speed;
                                                vg_clear_screen();
                                                vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                                
                                            } else {
                                                vg_clear_screen();
                                                vg_draw_pixmap(xf, y_init, map, pixmap_and_metadata);
                                                stopDrawing = true;
                                            }

                                        } else {
                                            stopDrawing = true;
                                        }
                                    } else {
                                        frames++;

                                        if (frames == ((unsigned int)-speed) && !stopDrawing && x_init > (int)xf) {
                                            vg_clear_screen();
                                            x_init--;
                                            vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                            frames = 0;
                                        }
                                        
                                    }
                                    /* End XNegativeAxis Logic */

                                }

                            } else { // y-axis movement.

                                 if (YPositiveAxis){ // Move along the positive x-axis.

                                    /* Begin XPositiveAxis Logic */
                                    if (speed >= 1){
                                        if (y_init <= yf && !stopDrawing) { // Drawing requires checking if the current pos is already the final pos.

                                            if (y_init + speed <= yf) { // Check if the next position is valid ( less than final position )
                                                y_init += speed;
                                                vg_clear_screen();
                                                vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                                
                                            } else {
                                                vg_clear_screen();
                                                vg_draw_pixmap(x_init, yf, map, pixmap_and_metadata);
                                                stopDrawing = true;
                                            }

                                        } else {
                                            stopDrawing = true;
                                        }
                                    } else {
                                        frames++;

                                        if (frames == ((unsigned int)-speed) && !stopDrawing && y_init < (int)yf) {
                                            vg_clear_screen();
                                            y_init++;
                                            vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                            frames = 0;
                                        }
                                        
                                    }
                                    /* End YPositiveAxis Logic */

                                } else {

                                    /* Begin YNegativeAxis Logic */
                                    if (speed >= 1){    
                                        if (y_init >= yf && !stopDrawing) { // Drawing requires checking if the current pos is already the final pos.

                                            if (y_init - speed >= (int)yf) { // Check if the next position is valid ( less than final position )
                                                y_init -= speed;
                                                vg_clear_screen();
                                                vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                                
                                            } else {
                                                vg_clear_screen();
                                                vg_draw_pixmap(x_init, yf, map, pixmap_and_metadata);
                                                stopDrawing = true;
                                            }

                                        } else {
                                            stopDrawing = true;
                                        }
                                    } else {
                                        frames++;

                                        if (frames == ((unsigned int)-speed) && !stopDrawing && y_init > (int)yf) {
                                            vg_clear_screen();
                                            y_init--;
                                            vg_draw_pixmap(x_init, y_init, map, pixmap_and_metadata);
                                            frames = 0;
                                        }
                                        
                                    }
                                    /* End YNegativeAxis Logic */
                                }
                            }
                            setCounter(0);
						}
                        
					}
					if (msg.m_notify.interrupts & kbd_int_bit) {
						kbc_ih();
                        if (getError()){
                            setError(false);
                            continue;
                        }
                        
                        uint8_t scanCode = getScancode(); // Fetch the scancode.
                        bool breakCode = (scanCode & 0x80);

                        // check if the scan code is a break code for ESC and end program
                        if (scanCode == ESC && breakCode) {stop = true;}
                        
                        switch (counter){
                        case 0:
                            scanCodes[counter] = scanCode;
                            if (scanCode == 0xe0){
                            counter++;
                            } else {
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            }

                            break;
                        case 1:
                            scanCodes[counter] = scanCode;
                            kbd_print_scancode(!breakCode, counter + 1, scanCodes);
                            counter = 0;
                            break;
                        default:
                            return -1;
                            break;
                        }
					}
                    break;
                default:
                    break; 
            }
        }
    }

	// Unsubscribe Timer 0 interrupts
	if (timer_unsubscribe_int() != 0){
		printf("Error in timer_unsubscribe_int()\n");
		return 1;
	}

	// Unsubscribe KBC interrupts
	if (unsubscribe_interrupts_kbd() != 0){
		printf("Error in kbd_unsubscribe_int()\n");
		return 1;
	}

    // Return to text mode.
    if (vg_exit() != 0){
		printf("Error in vg_exit()\n");
		return 1;
	}               

	return 0;
}

int(video_test_controller)() {
	/* To be completed */
	printf("%s(): under construction\n", __func__);

	return 1;
}
