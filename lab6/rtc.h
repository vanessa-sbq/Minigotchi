#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>



#define IRQ_RTC             8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define SECONDS 0
#define RTC_ALARM_SECONDS 1
#define MINUTES 2
#define RTC_ALARM_MINUTES 3
#define HOURS 4
#define RTC_ALARM_HOURS 5
#define WEEK_DAY 6
#define MONTH_DAY 7
#define MONTH 8
#define YEAR 9
#define RTC_REGA 10
#define RTC_REGB 11
#define RTC_REGC 12   

#define RTC_DC 0xC0

#define RTC_UIP BIT(7)
#define RTC_RS0 BIT(0)
#define RTC_RS1 BIT(1)
#define RTC_RS2 BIT(2)
#define RTC_RS3 BIT(3)
#define RTC_RS_CLEAR 0xf0

#define RTC_PIE BIT(6)
#define RTC_AIE BIT(5)
#define RTC_UIE BIT(4)

#define RTC_PF  BIT(6)
#define RTC_AF  BIT(5)
#define RTC_UF BIT(4)

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_week;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
    uint8_t reg_a;
    uint8_t reg_b;
    uint8_t reg_c;
} RTC_Config;

RTC_Config rtc_get_config(void); // Get the configuration from the RTC

int bcd_to_decimal(uint8_t * number); // Convert a BCD number to decimal
int binary_to_bcd(uint8_t * number); // Convert a binary number to BCD
int (rtc_subscribe_int)(uint8_t * bit_no);  // Subscribe the RTC interrupts
int(rtc_unsubscribe_int)(); // Unsubscribe the RTC interrupts
int (rtc_int_cause)(uint8_t * cause);   // Get the cause of the interrupt
int (enable_pie_int)(); // Enable the periodic interrupts
int (enable_aie_int)(); // Enable the alarm interrupts
int (enable_uie_int)(); // Enable the update interrupts
int (disable_pie_int)(); // Disable the periodic interrupts
int (disable_aie_int)(); // Disable the alarm interrupts
int (disable_uie_int)(); // Disable the update interrupts
int (rtc_set_alarm)(uint8_t sec, uint8_t min, uint8_t hour); // Set the alarm
int (init_rtc_vars)();  // Initialize the RTC variables
int (reset_rtc)();  // Reset the RTC
void (rtc_ih)(void);  // RTC interrupt handler
#endif
