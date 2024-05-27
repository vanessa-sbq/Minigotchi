#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "rtc.h"
#include "test6.h"



int main(int argc, char *argv[]) {
	rtc_test_date();
	rtc_test_int();

	return 0;
}

