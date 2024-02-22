#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Returns the LSB of a 2 byte integer.
 * 
 * @param val	input 2 byte integer
 * @param lsb	address of memory location to be updated with val's LSB
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
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
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
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
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
