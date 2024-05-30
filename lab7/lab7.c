#include <stdint.h>
#include <stdio.h>
#include <minix/endpoint.h>
#include <minix/sysutil.h>
#include <minix/sef.h>
#include <minix/type.h>

#include "test7.h"
#include "serial_port.h"
#include "serialPort.h"

unsigned char serial_read() {
	uint32_t received = 0;
	uint32_t status = 0;

	sys_inb(SERIAL_IRQ_1 + 0x05, &status);

	if (status & BIT(0)) {
		sys_inb(SERIAL_IRQ_1 + 0x00, &received);
		return (unsigned char) (received & 0xFF);
	}

	return 0;
}

int main(int argc, char *argv[]) {


	
	/* int init_var;
    endpoint_t ep;
    char name[256];
    int priv_f;

    // Initialize Service
    sef_startup();

    // Fetch our endpoint
    int result = sys_whoami(&ep, name, sizeof(name), &priv_f, &init_var);

    printf("Endpoint: %d, Name: %s, Privileges: %x, Init var: %d\n", ep, name, priv_f, init_var);


    // Enable IO-sensitive operations for ourselves
    result = sys_enable_iop(SELF); */   
    //serial_init();

	//lcf_start();
    sef_startup();
	sys_enable_iop(SELF);
	//sef_startup();
	serial_init();

    /* Enable IO-sensitive operations for ourselves */

	//serial_interrupt_handler();
	//serial_send_byte('A');
	//

	while(true) printf("%c", (char)serial_read());
	return 0;
}
