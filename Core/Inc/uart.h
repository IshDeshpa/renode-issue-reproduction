#ifndef __UART_H
#define __UART_H

#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void init_uart();

uint32_t uart_read(char *str);

uint32_t uart_write(char *str, uint32_t len);

extern void USART2_IRQHandler(void);

#endif
