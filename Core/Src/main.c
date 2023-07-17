#include "adc.h"
#include "uart.h"
#include <string.h>

static char read[20];

int main(){
    init_uart();
    init_adc();

    printf("Set ADC\n");

    while(strcmp(read, "Print ADC Value\n")){
        uart_read(read);
    }

    printf("10: %d", adc_get(CHAN_TEN));
    printf("11: %d", adc_get(CHAN_ELEVEN));

    printf("End Test");

    return 0;
}