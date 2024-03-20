#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8042.h"
#include "utils.h"

uint8_t data;
int hook_id;
uint32_t sysinb_cnt;
int hook_id2;
int counter;


/**
 * @brief Checks status read from the KBC
 * 
 * @return int 
 */
int kbd_check_status(){    
    uint8_t read_st;
    while( 1 ) {
        if (util_sys_inb(KBC_ST_REG, &read_st) != 0){
            printf("Error in util_sys_inb()\n");
            return -1;
        }
        /* loop while 8042 output buffer is empty */
        if( read_st & KBC_STAT_OBF ) {
            if (util_sys_inb(KBC_OUT_BUF, &data) != 0){
                printf("Error in util_sys_inb()\n");
                return -1;
            }
            if ( (read_st &(KBC_STAT_PAR | KBC_STAT_TIMEOUT)) != 0 ) {
                return -1;
            } else break;
        } 
    }
    return 0;
}

/**
 * @brief Handles keyboard interrupts (C implementation).
 * @brief Reads the status register and the output buffer (OB).
 * @brief If there was some error, the byte read from the OB should be discarded.
 * @brief All communication with other code must be done via global variables, static if possible.
 * @brief Must be defined using parenthesis around the function name.
 * 
 * @param - IH's take no arguments
 * 
 * @return IH's return no values.
 */
void (kbc_ih)() {
    // Read status register and check if there was some communiation error (and read output buffer)
    if (kbd_check_status() != 0){
        printf("Communication errors while reading kbc status\n");
        return;
    }
}



int kbd_enable_interrupts(){
    if (sys_outb(KBC_CMD_REG, KBC_READ_CB) != 0){
        printf("Error in sys_outb()\n");
        return -1;
    }

    uint8_t command_byte = 0;
    if (util_sys_inb(KBC_OUT_BUF, &command_byte) != 0){
        printf("Error in util_sys_inb()\n");
        return -1;
    }

    command_byte |= KBC_ENABLE_KBD_INT;

    if (sys_outb(KBC_CMD_REG, KBC_WRITE_CB) != 0){
        printf("Error in sys_outb()\n");
        return -1;
    }

    if (sys_outb(KBC_ARGS_REG, command_byte) != 0){
        printf("Error in sys_outb()\n");
        return -1;
    }

    return 0;
}


int keyboard_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL){
    printf("\nInvalid memory address for initialization!");
    return -1;
  }
  hook_id = 1;
  *bit_no = hook_id;  // Cast
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)){
    printf("error in sys_irqsetpolicy!\n");
    return -1;
  } 

  return 0;
}

int keyboard_unsubscribe_int() {
  if (sys_irqrmpolicy(&hook_id) != 0){
    printf("error in sys_irqrmpolicy!\n");
    return -1;
  } 
  return 0;
}


/* Timer functions */
void (timer_int_handler)() {
	counter++;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL){
        printf("\nInvalid memory address for initialization!");
        return -1;
    }
    hook_id2 = 0;
    *bit_no = hook_id2;
    if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id2))){
        printf("error in sys_irqsetpolicy!\n");
        return -1;
    } 
    return 0;
}

int (timer_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id2) != 0){
        printf("error in sys_irqrmpolicy!\n");
        return -1;
    } 
    return 0;
}
