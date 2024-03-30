#ifndef __TIMER_H_
#define __TIMER_H_

#include <stdbool.h>
#include <stdint.h>

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

#include "i8254.h"

#include <lcom/lcf.h>

// Getters and Setters
int getTimerCounter();
void setTimerCounter(int stc);

// Functions that deal with timer's subscription.
int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int)();

// Timer's interrupt handler
void (timer_int_handler)();

#endif /* __TIMER_H_ */
