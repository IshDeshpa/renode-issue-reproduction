#ifndef __ADC_H
#define __ADC_H

#include <ctype.h>
#include <stdint.h>

#define MAX_CHANNELS 10
#define ADC_PRECISION_BITS 12
#define ADC_RANGE_MILLIVOLTS 3300

typedef enum 
{
    CHAN_TEN, 
    CHAN_ELEVEN,
    NUMBER_OF_CHANNELS
} ADC_t;

void init_adc(void);

uint16_t adc_get(ADC_t hardwareDevice);

#endif
