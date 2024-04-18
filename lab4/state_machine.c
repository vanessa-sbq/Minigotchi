#include "state_machine.h"

static struct mouse_ev mouse_event;
static state_t st = INIT; // initial state; keep state

static bool terminate = false;

bool getTerminateCondition(){
  return terminate;
}
void setTerminateCondition(bool ter){
  terminate = ter;
}

void mouseEventInit(){
  mouse_event.type = BUTTON_EV;
  mouse_event.delta_x = 0;
  mouse_event.delta_y = 0;
}

// Begin State Machine
void check_hor_line(struct mouse_ev *evt, uint8_t x_min_len,  uint8_t tolerance) {
  static int x_len = 0;
  static int y_len = 0;

  switch (st) {
    case INIT:
      if(evt->type == LB_PRESSED){
        st = DRAW_L;
        x_len = 0;
        y_len = 0;
      } else {
        st = INIT;
      }
      break;
    case DRAW_L:
      if (evt->type == MOUSE_MOV) {

        if (evt->delta_x < 0 || evt->delta_y < 0){
          if ((-(evt->delta_x)) > tolerance || (-(evt->delta_y)) > tolerance){
            st = INIT;
            break;
          }
        }
        x_len += evt->delta_x;
        y_len += evt->delta_y;
      }

      if (evt->type == LB_RELEASED) {
        if (x_len == 0){
          st = INIT;
          break;
        }
        if (((y_len / x_len) < 1) || (x_len < x_min_len)){  // length requirements to change state
          st = INIT;
          break;
        }

        x_len = 0;
        y_len = 0;
        st = VERTEX;
      }
      break;
    case VERTEX:

      if (evt->type == RB_PRESSED){
        st = DRAW_R;
      } else {

        if (evt->type == MOUSE_MOV){
          if (evt->delta_x < 0 || evt->delta_y < 0){
              if ((-(evt->delta_x)) > tolerance || (-(evt->delta_y)) > tolerance){
                st = INIT;
                break;
              }
          } else {
            if (((evt->delta_x)) > tolerance || ((evt->delta_y)) > tolerance){
                st = INIT;
                break;
              }
          } 
          break;
        }

        if (evt->type == LB_PRESSED){
          st = DRAW_L;
          x_len = 0;
          y_len = 0;
          break;
        }

        st = INIT;
      }
      break;
    case DRAW_R:
      if (evt->type == MOUSE_MOV) {

        if (evt->delta_x < 0 || evt->delta_y > 0){
          if ((-evt->delta_x) > tolerance || (evt->delta_y) > tolerance){
            st = INIT;
            break;
          }
        }
        x_len += evt->delta_x;
        y_len += (-evt->delta_y);
      }

      if (evt->type == RB_RELEASED){
        if (x_len == 0){
          st = INIT;
          break;
        }
        if ( ((y_len / x_len) < 1) || (x_len < x_min_len)){ // check if length requirements not met
          st = INIT;
          break;
        }
        terminate = true;
      }
      break;
    default: 
      break;
  }
}

// End State Machine

struct mouse_ev *(mouse_det_event)(struct packet *pp){

  static struct packet temp = {
    .bytes[0] = 0,
    .bytes[1] = 0,
    .bytes[2] = 0,
    .rb = 0,
    .lb = 0,
    .mb = 0,
    .delta_x = 0,
    .delta_y = 0,
    .x_ov = 0,
    .y_ov = 0,
  };

  if (!temp.mb && pp->mb){ // Middle button was pressed.

    mouse_event.type = BUTTON_EV;
    temp.mb = pp->mb;
    return &mouse_event;

  } else if (temp.mb && !pp->mb) { // Middle button was released.

    mouse_event.type = BUTTON_EV;
    temp.mb = pp->mb;
    return &mouse_event;

  }

  if (!temp.lb && pp->lb){ // Left button was pressed.

    mouse_event.type = LB_PRESSED;
    temp.lb = pp->lb;
    return &mouse_event;

  } else if (temp.lb && !pp->lb) { // Left button was released.

    mouse_event.type = LB_RELEASED;
    temp.lb = pp->lb;
    return &mouse_event;

  }
  
  if (!temp.rb && pp->rb){ // Right button was pressed.

    mouse_event.type = RB_PRESSED;
    temp.rb = pp->rb;
    return &mouse_event;

  } else if (temp.rb && !pp->rb) { // Right button was released.

    mouse_event.type = RB_RELEASED;
    temp.rb = pp->rb;
    return &mouse_event;

  }
  return &mouse_event;
  

}
