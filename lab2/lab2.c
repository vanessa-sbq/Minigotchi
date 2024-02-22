#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/**
 * @brief Tests display of timer config.
 * @brief calls timer_get_conf() followed by timer_display_conf()
 * 
 * @param timer	Timer whose config to read (Ranges from 0 to 2)
 * @param field	Configuration field to display
 * 
 * @return Return 0 upon success and non-zero otherwise.
**/
int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}


/**
 * @brief Tests change of Timer O interrupt frequency.
 * @brief Programs Timer 0 to generate interrupts with input frequency
 * 
 * @param timer	Timer whose config to read (Ranges from 0 to 2)
 * @param freq	Frequency of interrupts to generate
 * 
 * @return Return 0 upon success and non-zero otherwise.
**/
int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

/**
 * @brief Tests Timer 0 interrupt handling.
 * @brief Subscribes Timer 0 interrupts and prints a message once per second for the specified time interval in seconds.
 * 
 * @param time	Length of time interval while interrupts are subscribed, in seconds
 * 
 * @return Return 0 upon success and non-zero otherwise.
**/
int(timer_test_int)(uint8_t time) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
