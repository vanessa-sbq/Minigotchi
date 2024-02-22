#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

/**
 * @brief Changes the operating frequency of a timer.
 * @brief Must use the read-back command so that it does not change the 4 LSBs (mode and BCD/binary) of the timer's control word.
 * 
 * @param bit_no	address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/**
 * @brief Subscribes and enables Timer 0 interrupts.
 * 
 * @param timer	Timer whose configuration to read (Ranges from 0 to 2)
 * @param st	Address of memory position to be filled with the timer config
 * 
 * @return Return 0 upon success and non-zero otherwise.
*/
int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/**
 * @brief Unsubscribes Timer 0 interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise.
*/
int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/**
 * @brief Timer 0 interrupt handler.
 * @brief Increments counter.
*/
void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

/**
 * @brief Reads the input timer configuration (status) via read-back command.
 * 
 * @param timer	Timer whose configuration to read (Ranges from 0 to 2).
 * @param st	Address of memory position to be filled with the timer config.
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/**
 * @brief Shows timer configuration.
 * @brief Displays, in a human friendly way, the specified field of a timer status, which was read via the read-back command.
 * 
 * @param timer	timer whose configuration should be displayed (Ranges from 0 to 2)
 * @param st	Address of memory position to be filled with the timer config
 * @param field	status field to display in human friendly way
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
