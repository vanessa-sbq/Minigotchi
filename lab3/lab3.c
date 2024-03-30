#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "timer.h"

#define ESC 0x81

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

int(kbd_test_scan)() {
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

                        // check if the scan code is a break code for ESQ and end program
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

    if (kbd_print_no_sysinb(getSysCallCounter()) != 0){
        return 1;
    }

    if (unsubscribe_interrupts_kbd() != 0){
        return 1;
    }

    return 0;
}

int(kbd_test_poll)() {
    bool stop = false;
    uint8_t scanCodes[2];
    uint8_t counter = 0;
    uint8_t status;
    uint8_t scanCode;
    
    while(!stop){
        
        if (readFromStatusKBC(&status) != 0) {
            continue;
        }

        if (readFromOutputBuffer(&scanCode) != 0){
            continue;
        }

        if ((status & (PARITY_STATUS_BIT | TIMEOUT_STATUS_BIT)) != 0){
            continue;
        }

        if ((status & (AUX_STATUS_BIT)) != 0){
            continue;
        }

        bool breakCode = (scanCode & 0x80);

        // check if the scan code is a break code for ESQ and end program
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
    
    if (kbc_enable_interrupts() != 0){
        return 1;
    }

    if (kbd_print_no_sysinb(getSysCallCounter()) != 0){
        return 1;
    }

    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
    bool stop = false;
    bool timeOut = false;
    uint8_t scanCodes[2];
    uint8_t counter = 0;
    
    uint8_t secondsBeingIdle = 0;

    uint8_t bit_no_kbd = 0;
    uint8_t bit_no_timer = 0;
    if (subscribe_interrupts_kbd(&bit_no_kbd) != 0){
        return 1;
    }
    
    if (timer_subscribe_int(&bit_no_timer) != 0){
        return 1;
    }

    uint8_t irq_set_kbd = (0x01 << bit_no_kbd);
    uint8_t irq_set_timer = (0x01 << bit_no_timer);
    
    int ipc_status, r;
    message msg;
    while( !stop && !timeOut ) { 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set_timer){
                        timer_int_handler();

                        if (getTimerCounter() - 60 == 0){
                            setTimerCounter(0);
                            secondsBeingIdle++;
                        }
                    }

                    if (secondsBeingIdle == n) {
                        timeOut = true;
                    }

                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbc_ih();
                        setTimerCounter(0);
                        secondsBeingIdle = 0;
                        
                        if (getError()){
                            setError(false);
                            continue;
                        }
                        
                        uint8_t scanCode = getScancode(); // Fetch the scancode.
                        bool breakCode = (scanCode & 0x80);

                        // check if the scan code is a break code for ESQ and end program
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

    if (kbd_print_no_sysinb(getSysCallCounter()) != 0){
        return 1;
    }

    if (unsubscribe_interrupts_kbd() != 0){
        return 1;
    }

    if (timer_unsubscribe_int() != 0){
        return 1;
    }

    return 0;
}
