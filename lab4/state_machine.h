#include <lcom/lcf.h>

/* Typedefs */
typedef enum {INIT, DRAW_L, VERTEX, DRAW_R, FINAL} state_t;

/* Setters and Getters */

bool getTerminateCondition();
void setTerminateCondition(bool ter);
void mouseEventInit();

/* Functions | State Machines */

struct mouse_ev *(mouse_det_event)(struct packet *pp);

void check_hor_line(struct mouse_ev *evt, uint8_t x_min_len,  uint8_t tolerance);
