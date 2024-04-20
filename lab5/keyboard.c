#include "keyboard.h"

static uint8_t scancode = 0x00;
static bool error = false;

// Getters and Setters
uint8_t getScancode(){
    return scancode;
}

void setScancode(uint8_t sc){
    scancode = sc; 
}

bool getError(){
    return error;
}

void setError(bool err){
    error = err; 
}


// Interrupt handler
void (kbc_ih)(){
    uint8_t status;
    if (readFromStatusKBC(&status) != 0) {
        error = true;
    }

    if (readFromOutputBuffer(&scancode) != 0){
        error = true;
    }

    if ((status & (PARITY_STATUS_BIT | TIMEOUT_STATUS_BIT)) != 0){
        error = true;
    }
}
