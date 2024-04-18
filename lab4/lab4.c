// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "timer.h"
#include "state_machine.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int16_t twosComplementToBinary(int16_t n) {

  // Sign extraction
  int8_t sign = ((n & 0x0100) >> 8) ;
    
  if (sign){ // If bit is set then it's a negative number (2's complement).

  n = ~n + 1; // Convert from 2's complement to normal representation.

  n = n & 0x01FF; // We are dealing with a 16 bit number...

  return -n;

  }

  return n;
}

int (mouse_test_packet)(uint32_t cnt) {

  /* Initialize the mouse */
  uint8_t bit_no_mouse;
  
  if (send_byte_to_mouse(MOUSE_ENABLE_DATA_REPORTING) != 0) {
    return 1;
  }
    
  if (mouse_subscribe_int(&bit_no_mouse) != 0) {
    return 1;
  }

  uint32_t irq_set = BIT(bit_no_mouse);

  /* Variables to store the mouse data. */
  //uint8_t packets[3];
  uint8_t counter = 0;
  struct packet pp;

  int ipc_status, r;
  int iters = cnt;
  while( iters > 0 ) {
    message msg;
    
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)){ 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if ((msg.m_notify.interrupts & irq_set )!= 0) {
            // Call the mouse interrupt handler.
            mouse_ih();

            // Store the read byte.
            uint8_t byte = get_readByte();

            if (byte == 0x00 && get_error()){ // An error occoured
              set_error(false);
              continue;
            }

            switch (counter){
              case 0:
                pp.bytes[counter] = byte; // Store the byte

                pp.lb = (byte & 0x01); // Left Button
                pp.rb = ((byte & 0x02) != 0); // Right Button
                pp.mb = ((byte & 0x04) != 0); // Middle Button

                pp.delta_x = (byte & 0x10) << 4; // 9th bit
                pp.delta_y = (byte & 0x20) << 3; // 9th bit

                pp.x_ov = ((byte & 0x40) != 0); // x overflow
                pp.y_ov = ((byte & 0x80) != 0); // y overflow
                break;
              case 1:
                pp.bytes[counter] = byte; // Store the byte
                pp.delta_x = pp.delta_x | byte; // Append the remaining 8 bits

                pp.delta_x = twosComplementToBinary(pp.delta_x);
                
                break;
              case 2:
                pp.bytes[counter] = byte; // Store the byte
                pp.delta_y = pp.delta_y | byte; // Append the remaining 8 bits

                pp.delta_y = twosComplementToBinary(pp.delta_y);

                break;
              default:
                printf("\nIllegal operation! Counter is greater or equal to 3.\n");
                break;
            }

            counter++;

            // Reset the byte counter and call the necessary functions.
            if (counter >= 3){
              iters--;
              mouse_print_packet(&pp);
              counter = 0;
            }
              
          }
          break;
        default:
          break;
      }
    } 
  }

  /* Deinitialize the mouse */

  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }
  if (send_byte_to_mouse(MOUSE_DISABLE_DATA_REPORTING) != 0){
    return 1;
  }
  
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    
  /* Initialize the mouse */
  uint8_t bit_no_mouse;

  /* Initialize the timer */
  uint8_t idleSeconds = 0;
  uint8_t bit_no_timer;

  if (timer_subscribe_int(&bit_no_timer) != 0) return 1;

  if (send_byte_to_mouse(MOUSE_ENABLE_DATA_REPORTING) != 0) return 1;
    
  if (mouse_subscribe_int(&bit_no_mouse) != 0) return 1;

  uint32_t irq_set = BIT(bit_no_mouse);
  uint32_t irq_set_timer = BIT(bit_no_timer);

  /* Variables to store the mouse data. */
  uint8_t counter = 0;
  struct packet pp;

  int ipc_status, r;
  
  while( idleSeconds != idle_time ) {
    message msg;
    
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)){ 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if ((msg.m_notify.interrupts & irq_set_timer )!= 0){
            timer_int_handler();

            if (getCounter() - sys_hz() == 0){
              setCounter(0);
              printf(".\n");
              idleSeconds++;
            }
          }

          if ((msg.m_notify.interrupts & irq_set )!= 0) {
            
            setCounter(0);
            idleSeconds = 0;
            
            // Call the mouse interrupt handler.
            mouse_ih();

            // Store the read byte.
            uint8_t byte = get_readByte();

            if (byte == 0x00 && get_error()){ // An error occoured
              set_error(false);
              continue;
            }

            switch (counter){
              case 0:
                pp.bytes[counter] = byte; // Store the byte

                pp.lb = (byte & 0x01); // Left Button
                pp.rb = ((byte & 0x02) != 0); // Right Button
                pp.mb = ((byte & 0x04) != 0); // Middle Button

                pp.delta_x = (byte & 0x10) << 4; // 9th bit
                pp.delta_y = (byte & 0x20) << 3; // 9th bit

                pp.x_ov = ((byte & 0x40) != 0); // x overflow
                pp.y_ov = ((byte & 0x80) != 0); // y overflow
                break;
              case 1:
                pp.bytes[counter] = byte; // Store the byte
                pp.delta_x = pp.delta_x | byte; // Append the remaining 8 bits

                pp.delta_x = twosComplementToBinary(pp.delta_x);
                
                break;
              case 2:
                pp.bytes[counter] = byte; // Store the byte
                pp.delta_y = pp.delta_y | byte; // Append the remaining 8 bits

                pp.delta_y = twosComplementToBinary(pp.delta_y);

                break;
              default:
                printf("\nIllegal operation! Counter is greater or equal to 3.\n");
                break;
            }

            counter++;

            // Reset the byte counter and call the necessary functions.
            if (counter >= 3){
              mouse_print_packet(&pp);
              counter = 0;
            }
              
          }
          break;
        default:
          break;
      }
    } 
  }

  /* Deinitialize the mouse */

  if (timer_unsubscribe_int() != 0) return 1;

  if (mouse_unsubscribe_int() != 0) return 1;
  if (send_byte_to_mouse(MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;
  
  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  // Initialize button event
  mouseEventInit();


  // Enable mouse data reporting
  uint8_t bit_no_mouse;
  if (send_byte_to_mouse(MOUSE_ENABLE_DATA_REPORTING) != 0) return 1;

  // Subscribe mouse interrupts
  if (mouse_subscribe_int(&bit_no_mouse) != 0) return 1;
  uint32_t irq_set = BIT(bit_no_mouse);

  // Interrupt loop
  uint8_t counter = 0;
  struct packet pp;

  int ipc_status, r;
  while(!getTerminateCondition()) {
    message msg;
    
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)){ 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if ((msg.m_notify.interrupts & irq_set )!= 0) {
            
            // Call the mouse interrupt handler.
            mouse_ih();

            // Store the read byte.
            uint8_t byte = get_readByte();

            if (byte == 0x00 && get_error()){ // An error occoured
              set_error(false);
              continue;
            }

            // Assemble packet 
            switch (counter){
              case 0:
                pp.bytes[counter] = byte; // Store the byte

                pp.lb = (byte & 0x01); // Left Button
                pp.rb = ((byte & 0x02) != 0); // Right Button
                pp.mb = ((byte & 0x04) != 0); // Middle Button

                pp.delta_x = (byte & 0x10) << 4; // 9th bit
                pp.delta_y = (byte & 0x20) << 3; // 9th bit

                pp.x_ov = ((byte & 0x40) != 0); // x overflow
                pp.y_ov = ((byte & 0x80) != 0); // y overflow
                break;
              case 1:
                pp.bytes[counter] = byte; // Store the byte
                pp.delta_x = pp.delta_x | byte; // Append the remaining 8 bits
                pp.delta_x = twosComplementToBinary(pp.delta_x);
                break;
              case 2:
                pp.bytes[counter] = byte; // Store the byte
                pp.delta_y = pp.delta_y | byte; // Append the remaining 8 bits
                pp.delta_y = twosComplementToBinary(pp.delta_y);
                break;
              default:
                printf("\nIllegal operation! Counter is greater or equal to 3.\n");
                break;
            }
            counter++;

            // Reset the byte counter and call the necessary functions.
            if (counter >= 3){
              mouse_print_packet(&pp);
              counter = 0;

              // Process packet (identify event)
              struct mouse_ev* evt = mouse_detect_event(&pp);
              
              // Send event to state machine
              check_hor_line(evt, x_len, tolerance);
            }
          }
          break;
        default: 
          break;
      }
    } 
  }

  // Unsubscribe mouse interrupts
  if (mouse_unsubscribe_int() != 0) return 1;

  // Disable mouse data reporting
  if (send_byte_to_mouse(MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
