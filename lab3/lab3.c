#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include "kbd.h"
#include <stdbool.h>
#include <stdint.h>
#include "utils.h"

extern uint8_t data;
extern uint32_t sysinb_cnt;
extern int counter;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


/**
 * @brief Tests reading of scancodes via KBD interrupts.
 * @brief Displays the scancodes received from the keyboard using interrupts.
 * @brief Exits upon release of the ESC key. 
 * @brief Must be defined using parenthesis around the function name.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbd_test_scan)() {
    uint8_t bit_no = 0;


    // subscribe to KBC interrupts
    if (keyboard_subscribe_int(&bit_no) != 0){
        printf("error in keyboard_subscribe_int()\n");
        return -1;
    }

    uint32_t irq_set = BIT(bit_no); // Mask to be used for msg.m_notify.interrupts
    int ipc_status = 0;
    message msg;
    int r = 0;
    bool escReleased = false;
    bool isTwoByteCode = false;
    uint8_t bytes[2]; // Data in case it has 2-byte scancodes
    bool make = false;
    bool completeScancode = false;


    while(!escReleased) { // You may want to use a different condition

        // Get a request message.
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        
        if (is_ipc_notify(ipc_status)) { // received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware interrupt notification				
                    if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
                        kbc_ih(); // process interrupt
                        tickdelay(DELAY_US);

                        
                        // Assemble scancode
                        uint8_t scancode = data;

                        if (scancode == 0xe0){ 
                            isTwoByteCode = true;
                            bytes[0] = scancode;
                            completeScancode = false;
                        }
                        make = ((scancode & BIT(7)) == 0); 

                        if (isTwoByteCode && scancode != 0xe0){ // read second byte from 2-byte scancode
                            bytes[1] = scancode;
                            completeScancode = true;
                        }
                        else if (!isTwoByteCode){
                            bytes[0] = scancode; 
                            completeScancode = true;
                        }

                        if (completeScancode){  // Ready to be printed
                            kbd_print_scancode(make, isTwoByteCode ? 2 : 1, bytes);
                            if (scancode == 0x81) escReleased = true; // function returns when releasing ESC key
                            completeScancode = false;
                            isTwoByteCode = false;
                        }
                    }
                    break;
                default:
                    break; // no other notifications expected: do nothing 	
            }
        } 
        else { // received a standard message, not a notification
            // no standard messages expected: do nothing
        }

    }

    // unsubscribe KBC interrupts
    if (keyboard_unsubscribe_int() != 0){
        printf("error in keyboard_unsubscribe_int()\n");
        return -1;
    }

    kbd_print_no_sysinb(sysinb_cnt); // Print number of sys_inb kernel calls

    return 0;
}


/**
 * @brief Tests reading of scancodes via polling.
 * @brief Displays the scancodes received from the keyboard using polling.
 * @brief Exits upon release of the ESC key.
 * @brief Must be defined using parenthesis around the function name.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbd_test_poll)() {
    bool escReleased = false;
    bool isTwoByteCode = false;
    uint8_t bytes[2]; // Data in case it has 2-byte scancodes
    bool make = false;
    bool completeScancode = false;

    while(!escReleased) { // You may want to use a different condition
        kbc_ih(); // process interrupt
        
        // Assemble scancode
        uint8_t scancode = data;

        if (scancode == 0xe0){ 
            isTwoByteCode = true;
            bytes[0] = scancode;
            completeScancode = false;
        }
        make = ((scancode & BIT(7)) == 0); 

        if (isTwoByteCode && scancode != 0xe0){ // read second byte from 2-byte scancode
            bytes[1] = scancode;
            completeScancode = true;
        }
        else if (!isTwoByteCode){
            bytes[0] = scancode; 
            completeScancode = true;
        }

        if (completeScancode){  // Ready to be printed
            kbd_print_scancode(make, isTwoByteCode ? 2 : 1, bytes);
            if (scancode == 0x81) escReleased = true; // function returns when releasing ESC key
            completeScancode = false;
            isTwoByteCode = false;
        }
    }

    if (kbd_enable_interrupts() != 0){
        printf("Error in kbd_enable_interrupts()\n");
        return -1;
    }
    kbd_print_no_sysinb(sysinb_cnt); // Print number of sys_inb kernel calls

    return 0;
}


/**
 * @brief Tests handling of more than one interrupt.
 * @brief Similar to kbd_test_scan() except that it should terminate also if no scancodes are
 *        received for n seconds.
 * @brief Must be defined using parenthesis around the function name.
 * 
 * @param n Number of seconds without scancodes before exiting.
 * 
 * @return Return 0 upon success and non-zero otherwise.
 */
int(kbd_test_timed_scan)(uint8_t n) {
    uint8_t bit_no = 1;
    uint8_t bit_no2 = 0;

    // subscribe to timer interrupts
    if (timer_subscribe_int(&bit_no) != 0){
        printf("error in timer_subscribe_int()\n");
        return -1;
    }

    // subscribe to KBC interrupts
    if (keyboard_subscribe_int(&bit_no2) != 0){
        printf("error in keyboard_subscribe_int()\n");
        return -1;
    }

    uint32_t timer0_int_bit = BIT(bit_no); // interrupt bit for timer
    uint32_t kbd_int_bit = BIT(bit_no2);   // interrupt bit for kbd
    int ipc_status = 0;
    message msg;
    int r = 0;
    bool escReleased = false;
    bool isTwoByteCode = false;
    uint8_t bytes[2]; // Data in case it has 2-byte scancodes
    bool make = false;
    bool completeScancode = false;
    bool keyPressed = false;
    counter = 0;
    int countdown = 0;
    bool leaveLoop = false;

    while(!escReleased && !leaveLoop) { // You may want to use a different condition
        // Get a request message.
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        
        if (is_ipc_notify(ipc_status)) { // received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware interrupt notification
                    if (msg.m_notify.interrupts & timer0_int_bit) { // timer interrupts
                        if (!keyPressed){
                            timer_int_handler();
                            if (60 - counter <= 0){
                                counter = 0;
                                countdown++;  // One second has passed
                                timer_print_elapsed_time();
                            }
                        }
                        else {
                            countdown = 0; // Reset countdown, because key was pressed
                        }
                        if (countdown  >= n){
                            leaveLoop = true;
                        }
                    }
                    if (leaveLoop) break;

                    if (msg.m_notify.interrupts & kbd_int_bit) { // keyboard interrupts
                        keyPressed = true;
                        counter = 0;
                        kbc_ih(); // process interrupt
                        //tickdelay(DELAY_US);
                        
                        // Assemble scancode
                        uint8_t scancode = data;

                        if (scancode == 0xe0){ 
                            isTwoByteCode = true;
                            bytes[0] = scancode;
                            completeScancode = false;
                        }
                        make = ((scancode & BIT(7)) == 0); 

                        if (isTwoByteCode && scancode != 0xe0){ // read second byte from 2-byte scancode
                            bytes[1] = scancode;
                            completeScancode = true;
                        }
                        else if (!isTwoByteCode){
                            bytes[0] = scancode; 
                            completeScancode = true;
                        }

                        if (completeScancode){  // Ready to be printed
                            kbd_print_scancode(make, isTwoByteCode ? 2 : 1, bytes);
                            if (scancode == 0x81) escReleased = true; // function returns when releasing ESC key
                            completeScancode = false;
                            isTwoByteCode = false;
                        }
                    } else keyPressed = false;
                    //tickdelay(DELAY_US);
                default:
                    break; // no other notifications expected: do nothing
                }
            }
    }   

    // unsubscribe timer interrupts
    if (timer_unsubscribe_int() != 0){
        printf("error in timer_unsubscribe_int()\n");
        return -1;
    }

    // unsubscribe KBC interrupts
    if (keyboard_unsubscribe_int() != 0){
        printf("error in keyboard_unsubscribe_int()\n");
        return -1;
    }

    return 0;
}
