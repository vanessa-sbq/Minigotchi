#include "utils.h"

// Setters and Getters
#ifdef LAB3
static int sysCallsCounter = 0;

int getSysCallCounter(){
    return sysCallsCounter;
}

void setSysCallCounter(int sscc){
    sysCallsCounter = sscc;
}

#endif


// Wrapper Function
int util_sys_inb(int port, uint8_t* read){
    uint32_t temp;

    if (sys_inb(port, &temp) != 0){
        return 1;
    }

    *read = (uint8_t)(temp);

    #ifdef LAB3
    sysCallsCounter++;
    #endif

    return 0;
}
