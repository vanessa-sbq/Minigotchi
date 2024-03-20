#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8042.h"


int hook_id;

/**
 * @brief Handles keyboard interrupts (C implementation).
 * @brief Reads the status register and the output buffer (OB).
 * @brief If there was some error, the byte read from the OB should be discarded.
 * @brief All communication with other code must be done via global variables, static if possible.
 * @brief Must be defined using parenthesis around the function name.
 * 
 * @param - IH's take no arguments
 * 
 * @return Returns IH's return no values.
 */
void (kbc_ih)();

int keyboard_subscribe_int(uint8_t *bit_no);

int keyboard_unsubscribe_int();

int keyboard_read_outbuf();

int kbd_enable_interrupts();


/* Keyboard functions */
void timer_ih();

void (timer_int_handler)();

int (timer_subscribe_int)(uint8_t *bit_no);

int (timer_unsubscribe_int)();

