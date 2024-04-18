#include <lcom/lcf.h>

#include "i8254.h"

// Getters and Setters

int getCounter();
void setCounter(uint8_t tc);

// Functions that help with the timer setup.

int (timer_subscribe_int)(uint8_t* bit_no);
int (timer_unsubscribe_int)();
void (timer_int_handler)();
