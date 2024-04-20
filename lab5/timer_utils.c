#include <lcom/lcf.h>
#include "timer_utils.h"

int hook_id_timer = 0;
static int timer_counter;

int getCounter(){
  return timer_counter;
}

void setCounter(int cnt) {
  timer_counter = cnt;
}

void (timer_int_handler)() {
  timer_counter++;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  if (bit_no == NULL){
    printf("\nError while executing func %s, error in timer_subscribe_int, found a NULL pointer.\n", __func__);
    return 1; 
  }

  *bit_no = hook_id_timer;

  if ( sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer)){
    printf("\nError while executing func %s, error while calling sys_irqsetpolicy.\n", __func__);
    return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_timer) != 0){
    printf("\nError while executing func %s, error while calling sys_irqrmpolicy.\n", __func__);
    return 1;
  }
  return 0;
}
