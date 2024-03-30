#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "kbc.h"

#include <lcom/lcf.h>

// Getters and Setters
uint8_t getScancode();
void setScancode(uint8_t sc);

bool getError();
void setError(bool err);

// Interrupt handler
void (kbc_ih)();

#endif /* __KEYBOARD_H */
