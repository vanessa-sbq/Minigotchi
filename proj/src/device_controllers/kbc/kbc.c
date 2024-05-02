#include "kbc.h"

static int kbc_hook_id = KBC_IRQ;

/**
 * @brief Subscribes and enables KBC interrupts.
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise.
 */
int subscribe_interrupts_kbd(uint8_t* bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = kbc_hook_id;
    if (sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbc_hook_id) != 0){
        return 1;
    }
    return 0;
}

/**
 * @brief Unsubscribes KBC interrupts.
 * @return Return 0 upon success and non-zero otherwise.
 */
int unsubscribe_interrupts_kbd(){
    if (sys_irqrmpolicy(&kbc_hook_id) != 0){
        return 1;
    }
    return 0;
}

/**
 * @brief Enables Keyboard interrupts.
 * @return Return 0 upon success and non-zero otherwise.
 */
int kbc_enable_interrupts(){
    uint8_t commandByte;
    if (writeToKBCPort(CMDS_PORT, READ_COMMAND_BYTE) != 0){
        return 1;
    }
    if (readFromOutputBuffer(&commandByte) != 0){
        panic("Error in kbc_enable_interrupts(), error reading KBC output buffer\n");
        return 1;
    }
    commandByte = commandByte | 0x01;
    if (writeToKBCPort(CMDS_PORT, WRITE_COMMAND_BYTE) != 0){
        panic("Error in kbc_enable_interrupts(), error writing command to KBC port\n");
        return 1;
    }
    if (writeToKBCPort(ARGS_PORT, commandByte) != 0){
        panic("Error in kbc_enable_interrupts(), error writing args to KBC\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Reads KBC status.
 * @return Return 0 upon success and non-zero otherwise.
 */
int readFromStatusKBC(uint8_t* status){
    if (util_sys_inb(STATUS_REG_PORT, status) != 0){
        return 1;
    }
    return 0;
}

/**
 * @brief Reads KBC output buffer.
 * @return Return 0 upon success and non-zero otherwise.
 */
int readFromOutputBuffer(uint8_t* byte){
    uint8_t status;
    uint8_t temp;

    while (1){
        if (readFromStatusKBC(&status) != 0){ // Read status from kbc.
            return 1;
        }

        if (status & OBF_STATUS_BIT){ // Must be full.

            if (util_sys_inb(OUT_BUF_PORT, &temp)){
                return 1;
            }
            
            *byte = temp;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

/**
 * @brief Writes to a KBC port.
 * @return Return 0 upon success and non-zero otherwise.
 */
int writeToKBCPort(uint8_t port, uint8_t data){
    uint8_t stat;
    while(1) {
        if (readFromStatusKBC(&stat) != 0){
            return 1;
        }

        if( (stat & IBF_STATUS_BIT) == 0 ) {
            if (sys_outb(port, data) != 0){
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}
