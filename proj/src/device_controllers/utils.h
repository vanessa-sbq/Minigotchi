#ifndef __UTILS_H
#define __UTILS_H

#include <lcom/lcf.h>

// Setters and Getters
#ifdef LAB3
int getSysCallCounter();
void setSysCallCounter(int sscc);
#endif

// Wrapper Function
int util_sys_inb(int port, uint8_t* read);

#endif /* __UTILS_H */
