//UART->asynchronous serial communication

/*
Read  and Display the configuration of the UART:
	- Both the serial communication parameters (LCR)
	- The interrupt events that are enabled (IER)	

	Test is using stty -a < /dev/tty00
*/
int ser_test_conf(unsigned short base_addr);

/*
Set the serial communication parameters of one serial port by writing to the LCR
	- How to test? Use test_conf()
		- Cannot use stty -a < dev/tty00 because it will change the configuration
*/
int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, 
	     long parity, /* -1: none, 0: even, 1: odd */
	     unsigned long rate);


/*
Send/receive strings using the serial port in:
	- polled mode
	-interrupt mode
Need to have both a sender and a receiver for testing ... use executable provided  with the solution

How to test? Use two VMs on the same PC, with the serial port of one connected to the sp of another
*/
int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 	unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);

int ser_test_int(/* details to be provided */) ;

int ser_test_fifo(/* details to be provided */);


