#include "timer.h"

static int hook_id_timer = TIMER0_IRQ;

static int timerCounter = 0;

// Getters and Setters

int getCounter(){
  return timerCounter;
}

void setCounter(uint8_t tc){
  timerCounter = tc;
}

// Functions that help with the timer setup.


int (timer_subscribe_int)(uint8_t* bit_no){
  if (bit_no == NULL) return 1;
  *bit_no = hook_id_timer;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != 0) return 1;
  return 0;
}

int (timer_unsubscribe_int)(){
  if (sys_irqrmpolicy(&hook_id_timer) != 0) return 1;
  return 0;
}

void (timer_int_handler)(){
  timerCounter++;
}
