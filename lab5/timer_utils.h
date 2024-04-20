#include <lcom/lcf.h>
#include "i8254.h"

void (timer_int_handler)();

int (timer_subscribe_int)(uint8_t *bit_no);

int (timer_unsubscribe_int)();

int getCounter();

void setCounter(int cnt);


