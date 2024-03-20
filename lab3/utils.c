#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.h"

/**
 * @brief Returns the LSB of a 2 byte integer.
 * 
 * @param val	input 2 byte integer
 * @param lsb	address of memory location to be updated with val's LSB
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  // if val = 1110 0101 0011 1111 => lsb = 0011 1111
  *lsb = (uint8_t) val;
  return 0;
}

/**
 * @brief Returns the MSB of a 2 byte integer.
 * 
 * @param val	input 2 byte integer
 * @param msb	address of memory location to be updated with val's LSB ? ... é como está online...
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  // if val = 1110 0101 0011 1111 => msb = 1110 0101
  val >>= 8; // 0000 0000 1110 0101
  *msb = (uint8_t) val;
  return 0;
}

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 * 
 * @param bit_no port	the input port that is to be read
 * @param value	address of 8-bit variable to be update with the value read
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t value32;
  if (sys_inb(port, &value32) != 0){
    printf("Error in sys_inb!\n");
    return -1;
  }

  #ifdef LAB3
  extern uint32_t sysinb_cnt;
  sysinb_cnt++;
  #endif


  *value = (uint8_t) value32;
  return 0;
}
