#include "uart.h"
#include "stm32f4xx.h"
#include <string.h>

static char read[20];

int main(){
    __disable_irq();
    init_uart();
    __enable_irq();

    printf("Start Test\n");

    while(1){
        uart_read(read);
        printf(read);
        printf("\n");
    }
    
    return 0;
}