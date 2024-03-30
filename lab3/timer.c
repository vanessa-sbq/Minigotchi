#include "timer.h"

static int hook_id = TIMER0_IRQ;
static int int_counter = 0;

// Getters and Setters
int getTimerCounter(){
    return int_counter;
}

void setTimerCounter(int stc){
    int_counter = stc;
}

// Functions that deal with timer's subscription.
int (timer_subscribe_int)(uint8_t *bit_no) {
	if (bit_no == NULL){
		return 1;
	}
	
	*bit_no = hook_id;

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0){
		return 1;
	}
	return 0;
}

// Unsubscribe timer interrupts
int (timer_unsubscribe_int)() {
	if (sys_irqrmpolicy(&hook_id) != 0){
		return 1;
	}
	return 0;
}

// Timer's interrupt handler
void (timer_int_handler)() {
	int_counter++;
}
