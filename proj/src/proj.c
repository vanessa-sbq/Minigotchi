#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

#define FPS 60 // TODO: Change to 60?

// Player data
#include "player_data/database.h"

// Device controllers
#include "device_controllers/video/video.h"
#include "device_controllers/kbc/keyboard.h"
#include "device_controllers/kbc/kbc.h"
#include "device_controllers/kbc/mouse.h"
#include "device_controllers/timer/timer.h"

// Models
#include "model/cursor.h"
#include "model/button.h"
#include "model/bar.h"
#include "model/stateModels/mainMenu.h"
#include "model/stateModels/mainRoom.h"
#include "model/stateModels/minigameMenu.h"
#include "model/stateModels/nameMinigotchi.h"

// Viewers
#include "viewer/guiDrawer.h"
#include "viewer/menus/mainMenuViewer.h"
#include "viewer/menus/mainRoomViewer.h"
#include "viewer/menus/minigameMenuViewer.h"
#include "viewer/menus/nameMinigotchiViewer.h"

// Controllers
#include "controller/menus/mainMenuController.h"
#include "controller/menus/mainRoomController.h"
#include "controller/menus/minigameMenuController.h"
#include "controller/menus/nameMinigotchiController.h"


// Database
static Database* database;


// TODO: Might need to add/remove some states
typedef enum {MAIN_MENU, MAIN_ROOM, NAME_MINIGOTCHI, MINIGAMES_WINDOW, MINIGAME_1, MINIGAME_2, EXIT} state_t;
static state_t game_state = MAIN_MENU; // Game's current state
static bool newGame = false; // Check if playing for the first time
//static char* minigotchi_name = "John Doe";


int main(int argc, char *argv[]) {
	lcf_set_language("EN-US");
	//lcf_trace_calls("/home/lcom/labs/g5/proj/src/trace.txt");  
	lcf_log_output("/home/lcom/labs/g5/proj/src/output.txt"); 
	if (lcf_start(argc, argv)) return 1;
	lcf_cleanup();
	return 0;
}

void proj_set_state(state_t state){
	game_state = state;
}

// TODO: FIX AND DON'T USE
int16_t twosComplementToBinary(int16_t n) {
	// Sign extraction
	int8_t sign = ((n & 0x0100) >> 8) ;
	if (sign){ // If bit is set then it's a negative number (2's complement).
		n = ~n + 1; // Convert from 2's complement to normal representation.
		n = n & 0x01FF; // We are dealing with a 16 bit number...
		return -n;
	}
	return n;
}

// TODO: FIX AND DON'T USE
int twosComplementToSignedInt(unsigned char value) {
    if (value & 0x80) {
        return (int)((unsigned int)value | 0xFFFFFF00); // Extend sign bit
    } else {
        return (int)value;
    }
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

	// Subscribe keyboard interrupts
    uint8_t kbd_bit_no = 0;
    if (subscribe_interrupts_kbd(&kbd_bit_no) != 0){
        return 1;
    }

	// Enable data reporting (mouse)
	if (send_byte_to_mouse(MOUSE_ENABLE_DATA_REPORTING) != 0) {
		return 1;
	}

	// Subscribe mouse interrupts
	uint8_t mouse_bit_no = 0;
	if (mouse_subscribe_int(&mouse_bit_no) != 0) {
    	return 1;
  	}

	uint8_t timer_bit_no = 0;

	if (timer_subscribe_int(&timer_bit_no) != 0) {
		return 1;
	}

	struct packet pp;
	uint8_t kbd_irq_set = BIT(kbd_bit_no);
	uint8_t mouse_irq_set = BIT(mouse_bit_no);
	uint8_t timer_irq_set = BIT(timer_bit_no);
	bool endGame = false;

	int ipc_status, r;

	// Get database instance
	database = new_database();
	if (database_check_file_exists()){ // Check if save file exists
		newGame = false; // Not playing for the first time
		database_load_from_file(database);
		
	} else {
		newGame = true; // Playing for the first time (database_check_file_exists() automatically creates new save file)
	}

	// These are variables that are going to be dynamically assigned depending on the game state.
	Cursor* cursor = new_cursor(100, 100);
	switchBackground(0);

	bool lockKeyboard = false;
    uint8_t scanCodes[2];
	
	scanCodes[0] = 0x00;
	scanCodes[1] = 0x00;	
	
    uint8_t counter = 0;
    uint8_t scanCodeCounter = 0;
	while(!endGame) {
		if (((getTimerCounter() - (sys_hz() / FPS)) == 0)){

			uint8_t scancode_first_byte = 0x00;
			uint8_t scancode_second_byte = 0x00;

			switch (game_state) {
				case NAME_MINIGOTCHI:
					nameMinigotchiController_step();
					nameMinigotchiViewer_draw();

					setNameMinigotchiCursor(cursor);

					if (nameMinigotchiController_getButtonEvent() == CONTINUE_NAMEMINIGOTCHI) {
						switchBackground(1);
						game_state = MAIN_ROOM;
						newGame = false;
						nameMinigotchiController_delete_nameMinigotchi();
						break;
					}

					if (nameMinigotchiController_getButtonEvent() == QUIT_NAMEMINIGOTCHI) {
						database_delete_file(database);
						nameMinigotchiController_delete_nameMinigotchi();
						endGame = true;
						break;
					}

					break;
				case MAIN_MENU:
					mainMenuController_step();
					mainMenuViewer_draw();
					
					setMainMenuCursor(cursor);

					if (mainMenuController_getButtonEvent() == START){ // Start the game
						mainMenuController_delete_mainMenu(); // Free the main menu
						if (newGame) {
							switchBackground(3);
							game_state = NAME_MINIGOTCHI;
							break;
						}
						switchBackground(1);
						game_state = MAIN_ROOM;	
					}

					if (mainMenuController_getButtonEvent() == QUIT){ // Quit the game
						mainMenuController_delete_mainMenu(); // Free the main menu
						endGame = true;
					}
					break;
				case MAIN_ROOM:
					mainRoomController_step();
					mainRoomViewer_draw();
					
					setMainRoomCursor(cursor);

					// We only want to read the keyboard if the driver receive loop has finished writting to the buffer.
					if (lockKeyboard) {
						scancode_first_byte = scanCodes[0];
						scancode_second_byte = scanCodes[1];
							
							if (scancode_first_byte == 0xe0 && scancode_second_byte == 0x4d) {
								mainRoomController_Hotbar_goRight();
							}
    						if (scancode_first_byte == 0xe0 && scancode_second_byte == 0x4b) {
								mainRoomController_Hotbar_goLeft();
							}
						

						// Call necessary function...
						if (scanCodes[0] == 0x12) {
							mainRoomController_toggleHotbar();
						}
						
						scanCodes[0] = 0x00;
						scanCodes[1] = 0x00;
						lockKeyboard = false;
					}

					if (mainRoomController_getButtonEvent() == MINIGAMES_MAINROOM){ // Open minigames 
						mainRoomController_setButtonEvent(NOP_MAINROOM);
						switchBackground(2);
						game_state = MINIGAMES_WINDOW;	
					}

					if (mainRoomController_getButtonEvent() == QUIT_MAINROOM){ // Quit the game
						mainRoomController_delete_mainRoom(); // Free the main menu
						endGame = true;	
					}
					break;
				case MINIGAMES_WINDOW: 
					minigameMenuController_step();
					minigameMenuViewer_draw();
					setMinigameMenuCursor(cursor);
					
					if (minigameMenuController_getButtonEvent() == QUIT_MINIGAMEMENU){ // Open minigames 
						minigameMenuController_setButtonEvent(NOP_MINIGAMEMENU);
						switchBackground(1);
						game_state = MAIN_ROOM;	
					}
					break;
				case MINIGAME_1:
					
					break;
				case MINIGAME_2:
					
					break;
				case EXIT:
					
					break;
				default:
					return 1;
					break;
			}
			setTimerCounter(0);
		}

		message msg;
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)){ 
		switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if ((msg.m_notify.interrupts & timer_irq_set) != 0) {
					timer_int_handler();
				}

				if ((msg.m_notify.interrupts & mouse_irq_set )!= 0) {
					mouse_ih();

					// Store the read byte.
					uint8_t byte = mouse_get_readByte();

					if (byte == 0x00 && mouse_get_error()){ // An error occoured
						mouse_set_error(false);
						continue;
					}

					switch (counter){
					case 0:
						pp.bytes[counter] = byte; // Store the byte

						pp.lb = (byte & 0x01); // Left Button
						pp.rb = ((byte & 0x02) != 0); // Right Button
						pp.mb = ((byte & 0x04) != 0); // Middle Button

						pp.delta_x = (byte & 0x10) << 4; // 9th bit
						pp.delta_y = (byte & 0x20) << 3; // 9th bit

						pp.x_ov = ((byte & 0x40) != 0); // x overflow
						pp.y_ov = ((byte & 0x80) != 0); // y overflow
						break;
					case 1:
						pp.bytes[counter] = byte; // Store the byte
						pp.delta_x = pp.delta_x | byte; // Append the remaining 8 bits
						pp.delta_x = twosComplementToBinary(pp.delta_x);

						break;
					case 2:
						pp.bytes[counter] = byte; // Store the byte
						pp.delta_y = pp.delta_y | byte; // Append the remaining 8 bits

						pp.delta_y = twosComplementToBinary(pp.delta_y);

						break;
					default:
						break;
					}

					counter++;

					// Reset the byte counter and call the necessary functions.
					if (counter >= 3){
						counter = 0;
    					int delta_x = pp.delta_x;
						int delta_y = pp.delta_y;

						if (true && cursor != NULL) {
							set_buttonClicked(pp.lb, cursor);
							if (!pp.x_ov) cursor_set_x(cursor, cursor_get_x(cursor) + delta_x);
							if (!pp.y_ov) cursor_set_y(cursor, cursor_get_y(cursor) - delta_y);	
						}

					}

				}

				if (msg.m_notify.interrupts & kbd_irq_set) {
                        kbc_ih();
                        
                        uint8_t scanCode = getScanCode(); // Fetch the scancode.
                        
                        switch (scanCodeCounter){
                        case 0:
                            if (!lockKeyboard) scanCodes[scanCodeCounter] = scanCode;

                            if (scanCode == 0xe0){
                            	scanCodeCounter++;
                            } else {
								lockKeyboard = true;
                            	scanCodeCounter = 0;
                            }

                            break;
                        case 1:
                            if (!lockKeyboard) scanCodes[scanCodeCounter] = scanCode;
							lockKeyboard = true;
                            scanCodeCounter = 0;
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

	// TODO: Remove (temp)
	delete_cursor(cursor);

	if (timer_unsubscribe_int() != 0) {
		return 1;
	}

	// Unsubscribe keyboard interrupts
	if (unsubscribe_interrupts_kbd() != 0){
        return 1;
    }

	// Unsubscribe mouse interrupts
	if (mouse_unsubscribe_int() != 0) {
    	return 1;
  	}

	// Disable data reporting (mouse)
	if (send_byte_to_mouse(MOUSE_DISABLE_DATA_REPORTING) != 0){
		return 1;
	}

    // Return to text mode.
    if (vg_exit() != 0){
		printf("Error in vg_exit()\n");
		return 1;
	}

    return 0;
}


