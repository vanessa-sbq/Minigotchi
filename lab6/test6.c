#include <minix/sysutil.h>
#include <minix/com.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "rtc.h"
extern bool alarm_wait;

int rtc_test_conf(void) {
    // Get the configuration from the RTC
    RTC_Config config = rtc_get_config();

    // Display the configuration
    printf("RTC Configuration:\n");
    printf("Seconds: %d\n", config.seconds);
    printf("Minutes: %d\n", config.minutes);
    printf("Hours: %d\n", config.hours);
    printf("Day of the Week: %d\n", config.day_of_week);
    printf("Day of the Month: %d\n", config.day_of_month);
    printf("Month: %d\n", config.month);
    printf("Year: %d\n", config.year);
    printf("Register A: %d\n", config.reg_a);
    printf("Register B: %d\n", config.reg_b);
    printf("Register C: %d\n", config.reg_c);

    return 0;
}

int rtc_test_int(/* to be defined in class */) { 
	rtc_ih();

    init_rtc_vars();

    enable_uie_int();
    enable_aie_int();

    rtc_set_alarm(0, RTC_DC, RTC_DC);

    uint8_t rtc_bit_no;

    int err = rtc_subscribe_int(&rtc_bit_no);
    if(err) return err;
    int rtc_irq_set = BIT(rtc_bit_no);


    int ipc_status, r;
    message msg;

    int sum = 0;

    while(alarm_wait) {
        if((r = driver_receive(ANY, &msg, &ipc_status))) {
            break;
        }

        if(is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)){
            case HARDWARE:
                if(msg.m_notify.interrupts & rtc_irq_set) {
                  rtc_ih();
                  sum ++;
                  if(sum == 10)
                    disable_uie_int();
                }
            default:
                break;
            }
        }
    }

    err = rtc_unsubscribe_int();
    if(err) return err;

    reset_rtc();

    return 0;
}
