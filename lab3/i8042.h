#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* The IRQ which the kbc raises for keyboard interrupt. */
#define KBC_IRQ 1

/* KBC Ports */
#define OUT_BUF_PORT 0x60
#define IN_BUF_PORT 0x60
#define STATUS_REG_PORT 0x64
#define CMDS_PORT 0x64
#define ARGS_PORT 0x60

/* Status bits */
#define PARITY_STATUS_BIT (0x01 << 7)
#define TIMEOUT_STATUS_BIT (0x01 << 6)
#define AUX_STATUS_BIT (0x01 << 5)
#define INH_STATUS_BIT (0x01 << 4)
#define A2_STATUS_BIT (0x01 << 3)
#define SYS_STATUS_BIT (0x01 << 2)
#define IBF_STATUS_BIT (0x01 << 1)
#define OBF_STATUS_BIT (0x01)

/* KBC Commands */
#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60

/* KBC time to wait between data */
#define DELAY_US 20000

#endif /* _LCOM_I8042_H */
