#include "rtc.h"

int rtc_hook_id = 5;
uint8_t binary_mode;
static uint8_t config_a;
static uint8_t config_b;
bool alarm_wait = true;

int rtc_subscribe_int() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

int bcd_to_decimal(uint8_t * number){
	uint8_t temp = 0;
	temp = (*number)&0xf;
	temp += 10*(((*number)>>4)&0xf);
	(*number) = temp;
	return 0;
}

int binary_to_bcd(uint8_t * number) {
    uint8_t temp = 0;
    temp = (*number)%10 | ((*number)/10) << 4;
    *number = temp;
    return 0;
}

int rtc_set_register(uint8_t reg){
	return sys_outb(RTC_ADDR_REG,reg);
}

int rtc_read_register(uint8_t reg, uint8_t * byte){
	rtc_set_register(reg);
	return util_sys_inb(RTC_DATA_REG, byte);
}

int rtc_write_reg(uint8_t reg, uint8_t byte) {
    rtc_set_register(reg);
    return sys_outb(RTC_DATA_REG, byte);
}

/*
What?: Display the date and time, in a human readable way
		> need not support all formats, only those the RTC is configured with


	EXAMPLE CODE : Waiting for Valid Time/Date
		void wait_valid_rtc(void) {
			unsigned long regA = 0;
			do {
			disable();
			sys_outb(RTC_ADDR_REG, RTC_REG_A);
			sys_inb(RTC_DATA_REG, &regA);
			enable();
			} while ( regA & RTC_UIP);
		}
		
	> assuming that functions enable() and disable() enable/disable interrupts
	> may not be what we  want, what if code is preempted? Or interrupted?

*/
int rtc_test_date(void) {
	uint8_t sec,min,hour,day,month,year;

	uint8_t config = 0;

	rtc_read_register(RTC_REGA, &config);
	

	// About to change date
	if(config&RTC_UIP)
		return 1;

	rtc_read_register(SECONDS,&sec);
	rtc_read_register(MINUTES,&min);
	rtc_read_register(HOURS,&hour);
	rtc_read_register(MONTH_DAY,&day);
	rtc_read_register(MONTH,&month);
	rtc_read_register(YEAR,&year);

	bcd_to_decimal(&sec);
	bcd_to_decimal(&min);
	bcd_to_decimal(&hour);
	bcd_to_decimal(&day);
	bcd_to_decimal(&month);
	bcd_to_decimal(&year);

	printf("Config: %d\n", config);

	printf("%d/%d/%d %d-%d-%d\n",day,month,year, hour,min,sec);

	return OK;
}

int (rtc_int_cause)(uint8_t * cause){
    int err = sys_outb(RTC_ADDR_REG, RTC_REGC);
    if(err) return err;

    return util_sys_inb(RTC_DATA_REG, cause);
}

int (enable_pie_int)(){
    uint8_t b_config;
    rtc_read_register(RTC_REGB, &b_config);
    return rtc_write_reg(RTC_REGB, b_config | RTC_PIE);
}

int (enable_aie_int)(){
    uint8_t b_config;
    rtc_read_register(RTC_REGB, &b_config);
    return rtc_write_reg(RTC_REGB, b_config | RTC_AIE);
}

int (enable_uie_int)(){
    uint8_t b_config;
    rtc_read_register(RTC_REGB, &b_config);
    return rtc_write_reg(RTC_REGB, b_config | RTC_UIE);
}

int (disable_pie_int)(){
    uint8_t b_config;
    rtc_read_register(RTC_REGB, &b_config);
    return rtc_write_reg(RTC_REGB, b_config & ~RTC_PIE);
}

int (disable_aie_int)(){
    uint8_t b_config;
    rtc_read_register(RTC_REGB, &b_config);
    return rtc_write_reg(RTC_REGB, b_config & ~RTC_AIE);
}

int (disable_uie_int)(){
    uint8_t b_config;
    rtc_read_register(RTC_REGB, &b_config);
    return rtc_write_reg(RTC_REGB, b_config & ~RTC_UIE);
}

int (rtc_set_alarm)(uint8_t sec, uint8_t min, uint8_t hour){
    if(sec  != RTC_DC) binary_to_bcd(&sec);
    if(min  != RTC_DC) binary_to_bcd(&min);
    if(hour != RTC_DC) binary_to_bcd(&hour);

    // ALARM EVERY MINUTE
    rtc_write_reg(RTC_ALARM_SECONDS, sec);
    rtc_write_reg(RTC_ALARM_MINUTES, min);
    return rtc_write_reg(RTC_ALARM_HOURS, hour);
}

int (init_rtc_vars)(){
    rtc_read_register(RTC_REGA, &config_a);
    return rtc_read_register(RTC_REGB, &config_b);
}

int (reset_rtc)(){
    rtc_write_reg(RTC_REGA, config_a);
    return rtc_write_reg(RTC_REGB, config_b);
}

void rtc_ih(void) {
    uint8_t cause;
    int err = rtc_int_cause(&cause);
    if(err) return; 

    if(cause & RTC_AF) {
        printf("Alarm");
        alarm_wait = false;
    }

    if(cause & RTC_PF) {
        printf("period");
    }

    if(cause & RTC_UF){
        rtc_test_date();
    }

}

RTC_Config rtc_get_config(void) {
    RTC_Config config;

    rtc_read_register(SECONDS, &config.seconds);
    rtc_read_register(MINUTES, &config.minutes);
    rtc_read_register(HOURS, &config.hours);
    rtc_read_register(WEEK_DAY, &config.day_of_week);
    rtc_read_register(MONTH_DAY, &config.day_of_month);
    rtc_read_register(MONTH, &config.month);
    rtc_read_register(YEAR, &config.year);
    rtc_read_register(RTC_REGA, &config.reg_a);
    rtc_read_register(RTC_REGB, &config.reg_b);
    rtc_read_register(RTC_REGC, &config.reg_c);

    return config;
}

