#include "mouse.h"

int hook_id = 12;

static uint8_t readByte;
static bool error;

/* 

  Functions that help get the static values.

*/
uint8_t get_readByte(){
  return readByte;
}

bool get_error(){
  return error;
}

void set_error(bool er){
  error = er;
}


/* 

  Functions that help with the communication of the kbc.

*/
int read_status_from_kbc(uint8_t* statusByte){
  if (statusByte == NULL){
    return 1;    
  }

  if (util_sys_inb(STATUS_REG_PORT, statusByte) != 0){
      return 1;
  }
  return 0;
}

int read_from_out_buf_kbc(uint8_t* storeByte){
  uint8_t statusByte; // Store status byte.

  if (storeByte == NULL){
    return 1;    
  }

  while (1){
    if (read_status_from_kbc(&statusByte) != 0){ // Fetch the status byte.
      return 1; // Error
    }

    if (statusByte & OBF_BIT){ // Output buffer must be full.
      if (util_sys_inb(OUT_BUF, storeByte) != 0){
        return 1; // Error
      }
      return 0;
    }
  }

}

int issueCommandKBC(uint8_t port, uint8_t sendToPort){
  uint8_t statusByte;
  while(1) {
    if (util_sys_inb(STATUS_REG_PORT, &statusByte) != 0){
      return 1;
    }

    if((statusByte & IBF_BIT) == 0 ) {
      sys_outb(port, sendToPort);
      return 0; 
    }
    
  }
}

/* 

  Functions that activate/deactivate the necessary things for mouse to work.

*/
int (mouse_subscribe_int)(uint8_t* bit_no){
  if (bit_no == NULL){
    printf("\nError while executing keyboard_subscribe_int, bit_no is NULL.\n");
    return 1;
  }

  *bit_no = hook_id;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) ){
    printf("\nError while executing sys_irqsetpolicy.\n");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)(){
  if (sys_irqrmpolicy(&hook_id) != 0){
    printf("\nError while exectuing keyboard_unsubscribe_int.\n");
    return 1;
  }
  return 0;
}

int send_byte_to_mouse(uint8_t commandToSend){

  uint8_t out_buf_byte = 0xFE;
  int tries = 10;

  while( out_buf_byte != ACK && tries >= 0 ) {

    // Tell kbc we want to send a command to the mouse.
    if (issueCommandKBC(CMDS_REG_PORT, MOUSE_WRITE_COMMAND) != 0) { 
      return 1;
    }

    // Send the command to the mouse.
    if (issueCommandKBC(ARGS_REG_PORT, commandToSend) != 0) {
      return 1;
    }

    tickdelay(micros_to_ticks(20000));
    
    // Read the acknoledgment byte.
    if (util_sys_inb(0x60, &out_buf_byte) != 0) return 1;

    // Only return if no error occoured.
    tries--;

  }
  return 0;
}

/* 

  The interrupt handler for the mouse.

*/
void (mouse_ih)(){
  if (read_from_out_buf_kbc(&readByte) != 0){ // Something went wrong.
    readByte = 0x00;
    error = true;
  }
}

