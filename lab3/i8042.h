#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/* Keyboard IRQ line */
#define KEYBOARD_IRQ 1
 
/* Scancodes */
#define ESC_MAKE 0x81

/* KBC Registers */
#define KBC_ST_REG  0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_ARGS_REG 0x60

/* KBC Status Register bits */
#define KBC_STAT_PAR     BIT(7) // Parity error - invalid data
#define KBC_STAT_TIMEOUT BIT(6) // Timeout error - invalid data
#define KBC_STAT_AUX     BIT(5) // Mouse data
#define KBC_STAT_INH     BIT(4) // Inhibit flag: 0 if keyboard is inhibited
#define KBC_STAT_A2      BIT(3) // A2 input line: irrelevant for LCOM
#define KBC_STAT_SYS     BIT(2) // System flag: irrelevant for LCOM
#define KBC_STAT_IBF     BIT(1) // Input buffer full, don't write commands or arguments
#define KBC_STAT_OBF     BIT(0) // Output buffer full - data available for reading

/* KBC time to wait between data */
#define DELAY_US 20000

/* KBC Commands */
#define KBC_READ_CB 0x20 // Read command byte
#define KBC_WRITE_CB 0x60 // Write command byte

/* KBC Command Byte */
#define KBC_ENABLE_KBD_INT BIT(0) // Enable interrupt on OBF, from keyboard

/* Make or break code bit */
#define MAKE_BREAK BIT (7)


/* Timer Macros */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */


#endif /* _LCOM_I8254_H */
