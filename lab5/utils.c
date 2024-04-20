#include "utils.h"


// Wrapper Function
int util_sys_inb(int port, uint8_t* read){
    uint32_t temp;
    if (sys_inb(port, &temp) != 0){
        return 1;
    }
    *read = (uint8_t)(temp);
    return 0;
}
