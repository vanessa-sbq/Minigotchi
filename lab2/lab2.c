#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

int timer_counter = 0;

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
  uint8_t *st = (uint8_t*)(malloc(sizeof(uint8_t)));
  if (timer_get_conf(timer, st) != 0){
    printf("\nError while executing func %s, error in timer_get_conf", __func__);
    return 1;
  }

  if (timer_display_conf(timer, (*st), field) != 0){
    printf("\nError while executing func %s, error in timer_display_conf", __func__);
    return 1;
  }

  return 0;
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
  if (timer_set_frequency(timer, freq) != 0){
    printf("\nError while executing func %s, error while calling timer_set_frequency.\n", __func__);
    return 1;
  }
  return 0;
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

  uint8_t bit_number;
  if (timer_subscribe_int(&bit_number) != 0){
    printf("\nError while executing func %s, error while calling timer_subscribe_int.\n", __func__);
    return 1;
  }
  uint32_t irq_set = BIT(bit_number); // Create a mask to be used for bit extraction in msg.m_notify.interrupts.
  
  int ipc_status;
  int r;
  while(time > 0) { /* You may want to use a different condition */
  message msg; /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if ((msg.m_notify.interrupts & irq_set )!= 0) { /* subscribed interrupt */
                  timer_int_handler(); // Call the handler function.
                   
                  if (timer_counter - 60 == 0){ // According to timer 0's config, every time we reach 60 a second passes.
                    timer_counter = 0; // Reset the timer.
                    time--; // One second has passed
                    timer_print_elapsed_time(); // Print the elapsed time.
                  }

                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }

  if (timer_unsubscribe_int() != 0){ // Unsubscribe the interrupts of timer 0.
    printf("\nError while executing func %s, error while calling timer_unsubscribe_int.\n", __func__);
    return 1;
  }
  return 0;
}
