#include "adc.h"
#include "uart.h"
#include "stm32f4xx.h"
#include <string.h>

static char read[20];

int main(){
    __disable_irq();
    init_uart();
    init_adc();
    __enable_irq();

    printf("Set ADC\n");

    while(1){
        uart_read(read);
        printf(read);
        printf("\n");
    }

    printf("10: %d", adc_get(CHAN_TEN));
    printf("11: %d", adc_get(CHAN_ELEVEN));

    printf("End Test");

    return 0;
}