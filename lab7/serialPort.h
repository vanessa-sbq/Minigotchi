#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include <stdint.h>
#include <stdbool.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

#include "queue.h"
#include "serial_port.h"        //macros

#define MAX_ATTEMPTS 5


//Subscribes to serial port interrupts with REENABLE and EXCLUSIVE policies.

int serial_subscribe_int(uint8_t* bit_no);

//Unsubscribes from serial port interrupts.

int serial_unsubscribe_int();

//Retrieves the current status of the serial port.

int serial_get_status(uint8_t* status);

//Initializes the serial port, enabling interrupts and creating the data reception queue.

void serial_init();

//Cleans up and exits the serial port, clearing the data reception queue.

void serial_exit();

//Serial port interrupt handler.

void serial_interrupt_handler();

// Sends a byte through the serial port.

int serial_send_byte(uint8_t byte);

// Reads a byte from the serial port and pushes it to the reception queue.

int serial_read_byte();

//Retrieves the queue that stores received data.

struct Queue* serial_get_receive_queue();

//Prepares the queue and FIFO Control Register for the next interrupt, clearing the queue to prevent errors.

int serial_clear_interrupts();

//Processes a received byte during the waiting state in guess mode.

int process_waiting_guess(uint8_t val);

//Processes a received byte during the loading state in guess mode.

int process_loading_guess(uint8_t val);

// Processes a received byte during the playing state in guess mode.

int process_playing_guess(uint8_t val);

#endif
