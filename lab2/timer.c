#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id;
extern int timer_counter; // This helps us acess the variable that was defined inside lab2.c

/**
 * @brief Changes the operating frequency of a timer.
 * @brief Must use the read-back command so that it does not change the 4 LSBs (mode and BCD/binary) of the timer's control word.
 *  
 * @param timer	Timer to configure. (Ranges from 0 to 2)
 * @param freq	Timer operating frequency
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  /*
  
  The value that will be sent to the timer register needs to be less or equal to 16 bits.

  Our upper limit is 2**16 which is 65536, so counter_value < 2**16.

  Our lower limit is counter_value > 0.

  To obtain this counter_value we have to apply the following expression clock/div.

  We should check if these conditions first.

  So, for couter_value to be > 0, freq has to be less or equal to TIMER_FREQ value.

  For counter_value to be <= 2**16:
   
  counter_value <= 2**16
  TIMER_FREQ / freq <= 2**16
  (freq)**-1 <= 2**16 / TIMER_FREQ
  freq >= TIMER_FREQ / 2**16
  Giving us freq >= 19 
  
  */

  if (freq < 19 || freq > TIMER_FREQ){
    printf("\nError while executing func %s, cannot convert counter_value. freq (%u) is out of range\n", __func__, freq);
    return 1;
  }

  uint16_t counter_value = TIMER_FREQ / freq;

  uint8_t counter_value_LSB;
  util_get_LSB(counter_value, &counter_value_LSB);

  uint8_t counter_value_MSB;
  util_get_MSB(counter_value, &counter_value_MSB);

  uint8_t st;

  // Fetch timer's configuration
  if (timer_get_conf(timer, &st) != 0){
    printf("\nError while executing func %s, error in timer_set_frequency.\n", __func__);
    return 1;
  }

  uint32_t port; // This will be the hex value of the timer's port.

  uint32_t byte = 0; // Control-Word -> It will be the word that issues the write-back command.

  byte = byte ^ byte; // Make sure every bit is set to 0;


  switch (timer){
    case 0:
      port = TIMER_0;
      break;
    case 1:
      port = TIMER_1;
      byte = byte | 0x40; // Select timer 1
      break;
    case 2:
      port = TIMER_2;
      byte = byte | 0x80; // Select timer 2
      break;
    default:
      printf("%s No port: %d", __func__, timer);
      return 1;
  }

  byte = byte | ((st) & 0x0F); // Restore the timer's base and couting mode.

  byte = byte | (0x30); // Let's use MSB after LSB mode.

  // Send write command to Timer's control register.
  if (sys_outb(TIMER_CTRL, byte) != 0){
    printf("\nError while executing func %s, first sys_outb\n", __func__);
    return 1;
  }

  if (sys_outb(port, counter_value_LSB) != 0){
    printf("\nError while executing func %s, unable to send LSB\n", __func__);
    return 1;
  }

  if (sys_outb(port, counter_value_MSB) != 0){
    printf("\nError while executing func %s, unable to send MSB\n", __func__);
    return 1;
  }

  return 0;
}

/**
 * @brief Subscribes and enables Timer 0 interrupts.
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * 
 * @return Return 0 upon success and non-zero otherwise.
*/
int (timer_subscribe_int)(uint8_t *bit_no) {

  //printf("Hook_ID before irqsetpolicy: %d \n", hook_id);

  if (bit_no == NULL){
    printf("\nError while executing func %s, error in timer_subscribe_int, found a NULL pointer.\n", __func__);
    return 1; 
  }

  if ((*bit_no = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id))){
    printf("\nError while executing func %s, error while calling sys_irqsetpolicy.\n", __func__);
    return 1;
  }

  //printf("Hook_ID after irqsetpolicy: %d \n", hook_id);

  return 0;
}

/**
 * @brief Unsubscribes Timer 0 interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise.
*/
int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0){
    printf("\nError while executing func %s, error while calling sys_irqrmpolicy.\n", __func__);
    return 1;
  }
  return 0;
}

/**
 * @brief Timer 0 interrupt handler.
 * @brief Increments counter.
*/
void (timer_int_handler)() {
  timer_counter++;
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

  if (st == NULL){
    printf("\nError while executing func %s, error in timer_get_conf, found a NULL pointer.\n", __func__);
    return 1; 
  }

  int port; // This will be the hex value of the timer's port.

  uint32_t byte = 0; // Control-Word -> It will be the word that issues the read-back command.

  byte = byte ^ byte; // Make sure every bit is set to 0;

  byte = byte | TIMER_RB_CMD; // 11000000

  switch (timer){
    case 0:
      port = TIMER_0;
      byte = byte | TIMER_RB_SEL(0); // 11000010
      break;
    case 1:
      port = TIMER_1;
      byte = byte | TIMER_RB_SEL(1); // 11000100
      break;
    case 2:
      port = TIMER_2;
      byte = byte | TIMER_RB_SEL(2); //11001000
      break;
    default:
      printf("%s No port: %d", __func__, timer);
      return 1;
  }

  byte = byte | TIMER_RB_COUNT_; // 1110[1000]
  
   // Write control-word to the timer's controller.
  if (sys_outb(TIMER_CTRL, byte) !=0){
    printf("\nError while executing func %s, error in sys_outb.\n", __func__);
    return 1;
  } 

  // Write timer config.
  if (util_sys_inb(port, st)){
    printf("\nError while executing func %s, error in util_sys_inb.\n", __func__);
    return 1; 
  }

  return 0;
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
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  uint8_t st_modf = st; // Get ready to modify st value.

  union timer_status_field_val tsfl; // Initialize a union.

  // Determine what the union should hold.
  switch (field){
    case tsf_all:
      tsfl.byte = st; // Set status byte.
      break;
    case tsf_initial:
		// Get the initialization mode and right shift by four bits to facilitate access.
		st_modf = (st_modf & TIMER_LSB_MSB) >> 4;

		// Initialization modes according to intel.
		switch (st_modf){
			case 0x00:
				tsfl.in_mode = INVAL_val;
				break;
			case 0x01:
				tsfl.in_mode = LSB_only;
				break;
			case 0x02:
				tsfl.in_mode = MSB_only;
				break;
			case 0x03:
				tsfl.in_mode = MSB_after_LSB;
				break;
			default:
				printf("\nError while executing func %s, invalid initialization mode found.\n", __func__);
				return 1;
		}
		break;

    case tsf_mode:
      st_modf = (st_modf & (TIMER_SQR_WAVE | BIT(3))) >> 1; // Get the counting mode and right shift by one bit to facilitate access.

	  // Counting modes according to intel.
      switch (st_modf){
      case 0x00:
        tsfl.count_mode = 0;
        break;
      case 0x01:
        tsfl.count_mode = 1;
        break;
      case 0x02:
        tsfl.count_mode = 2;
        break;
      case 0x03:
        tsfl.count_mode = 3;
        break;
      case 0x04:
        tsfl.count_mode = 4;
        break;
      case 0x05:
        tsfl.count_mode = 5;
        break;
        // These cases are mutations of x10 and x11. According to the specification x
        //  should be 0 to mantain future compatibility with intel products.
        // But this does not mean the test cases will not set x to 1. In this case we should
        //  treat them as counting mode 2 and 3 respectivly.
      case 0x06:
        tsfl.count_mode = 2;
        break;
      case 0x07:
        tsfl.count_mode = 3;
        break;
        break;      
      default:
        printf("\nError while executing func %s, invalid count mode found, %x\n", __func__, st_modf);
        return 1;
  	}
      break;
    case tsf_base:
      tsfl.bcd = st_modf & TIMER_BCD; // BCD Mask and tsfl.bcd bool assignement.
      break;
  }

  // Call the print config function.
  if (timer_print_config(timer, field, tsfl) != 0){
    printf("\nError while executing func %s, timer_print_config step.\n", __func__);
    return 1;
  }

  return 0;
}
