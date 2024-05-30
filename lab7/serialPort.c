#include "serialPort.h"
#include <lcom/utils.h>


int serial_hook_id = 7;
static struct Queue *receive_queue;


int serial_subscribe_int(uint8_t* bit_no){
    *bit_no = BIT(serial_hook_id);
    if(sys_irqsetpolicy(SERIAL_IRQ_1, IRQ_REENABLE | IRQ_EXCLUSIVE, &serial_hook_id) != 0){
        printf("Error in sys_irqsetpolicy()\n");
        return 1;
    }
    return 0;
}

int serial_unsubscribe_int(){
    if(sys_irqrmpolicy(&serial_hook_id) != 0){
        printf("Error in sys_irqrmpolicy()\n");
        return 1;
    }
    return 0;
}

int serial_get_status(uint8_t* status){
    return util_sys_inb(SERIAL_PORT + LINE_STATUS, status);
}

void serial_init(){
    uint32_t temp;

    uint8_t lcr;

    if(sys_inb(SERIAL_PORT + INTERRUPT_ENABLE, &temp) != 0){
        printf("Error in utils_sys_inb()\n");
        return; //acaba
    }

    lcr = (uint8_t)(temp);

    lcr &= 0xF0;

    if(sys_outb(SERIAL_PORT + INTERRUPT_ENABLE, lcr | IER_DATA_AVAIL_INT) != 0){
        printf("Error in sys_outb()\n");
        return; //acaba
    }
    
    receive_queue = createQueue();
}

void serial_exit(){
    free(receive_queue);
}

void serial_interrupt_handler(){
    uint8_t ir;

    uint32_t temp;

    if(sys_inb(SERIAL_PORT + INTERRUPT_ID, &temp) != 0){
        printf("Error in util_sys_inb()\n");
        return; //acaba
    }

    ir = (uint8_t)temp;

    if(!(ir & IIR_NO_INTERRUPT)){
        if(ir & IIR_INTERRUPT_ID == IIR_CHAR_TIMEOUT){
            while(!serial_read_byte());
        }
    }
}

int serial_send_byte(uint8_t byte){
    uint8_t st;
    uint8_t attmps = 10;

    while(attmps--){
        if(serial_get_status(&st)) return 1;
        if(st & LSR_THR_EMPTY){return sys_outb(SERIAL_PORT + TRANSMIT_HOLD, byte);}
    }
    return 1;
}

int serial_read_byte(){
    uint8_t st;
    uint8_t byte;

    uint32_t temp;

    if(serial_get_status(&st)) return 1;
    if(st & LSR_DATA_READY){
        if(sys_inb(SERIAL_PORT + RECEIVE_BUFFER, &temp)) return 1;
        byte = (uint8_t)(temp);
        if(!(st & (LSR_OVERRUN_ERROR | LSR_PARITY_ERROR | LSR_FRAMING_ERROR))){
            enqueue(receive_queue, byte);
            return 0;
        }
    }
    return 1;
}

struct Queue* serial_get_receive_queue(){
    return receive_queue;
}

int serial_clear_interrupts(){
    if(sys_outb(SERIAL_PORT + FIFO_CONTROL, FCR_CLEAR) != 0){
        printf("Error in sys_outb()\n");
        return 1;
    }
    while(!isEmpty(receive_queue)){
        dequeue(receive_queue);
    }

    return 0;
}

int process_waiting_guess(uint8_t val);

int process_loading_guess(uint8_t val);

int process_playing_guess(uint8_t val);
