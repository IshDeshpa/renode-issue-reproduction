#include "uart.h"
#include "stm32f4xx.h"
#include <string.h>

static char read[20];

int main(){
    __disable_irq();
    init_uart();
    __enable_irq();
    
    // for(volatile int i=0; i<50000; i++);    // delay

    printf("Start Test\n\r");

    uart_read(read);

    // for(volatile int i=0; i<50000; i++);    // delay

    printf(read);
    printf("\r\n");
    
    return 0;
}