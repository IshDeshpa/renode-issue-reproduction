/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "uart.h"
#include "stm32f4xx.h"

#define TX_SIZE     128
#define RX_SIZE     64

// Initialize the FIFOs

#define FIFO_TYPE char
#define FIFO_SIZE TX_SIZE
#define FIFO_NAME txfifo
#include "fifo.h"
static txfifo_t usbTxFifo;

#define FIFO_TYPE char
#define FIFO_SIZE RX_SIZE
#define FIFO_NAME rxfifo
#include "fifo.h"
static rxfifo_t usbRxFifo;

static bool usbLineReceived = false;

void init_uart() {
    usbTxFifo = txfifo_new();
    usbRxFifo = rxfifo_new();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    USART_InitTypeDef UART_InitStruct = {0};

    // Initialize clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);

    // Initialize pins
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    //Initialize UART2
    UART_InitStruct.USART_BaudRate = 115200;
    UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    UART_InitStruct.USART_Parity = USART_Parity_No;
    UART_InitStruct.USART_StopBits = USART_StopBits_1;
    UART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &UART_InitStruct);

    USART_Cmd(USART2, ENABLE);

    // Enable NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    setvbuf(stdout, NULL, _IONBF, 0);
}

uint32_t uart_read(char *str) {
    char data = 0;
    uint32_t recvd = 0;

    bool *line_recvd = &usbLineReceived;
    
    while(*line_recvd == false){
        uart_write("", 0);   // needs to be in. Otherwise, usbLineRecieved will not update
    }

    USART_TypeDef *usart_handle = USART2;

    USART_ITConfig(usart_handle, USART_IT_RXNE, RESET);
    
    rxfifo_t *fifo = &usbRxFifo;
    
    rxfifo_peek(fifo, &data);
    while(!rxfifo_is_empty(fifo) && data != '\r') {
        recvd += rxfifo_get(fifo, (char*)str++);
        rxfifo_peek(fifo, &data);
    }

    rxfifo_get(fifo, &data);
    *str = 0;
    *line_recvd = false;
    USART_ITConfig(usart_handle, USART_IT_RXNE, SET);

    return recvd;
}

uint32_t uart_write(char *str, uint32_t len) {
    uint32_t sent = 0;

    USART_TypeDef *usart_handle = USART2;
    
    USART_ITConfig(usart_handle, USART_IT_TC, RESET);

    txfifo_t *fifo = &usbTxFifo;

    while(sent < len) {
        if(!txfifo_put(fifo, str[sent])) {
            // Allow the interrupt to fire
            USART_ITConfig(usart_handle, USART_IT_TC, SET);
            // Wait for space to open up
            while(txfifo_is_full(fifo));
            // Disable the interrupt again
            USART_ITConfig(usart_handle, USART_IT_TC, RESET);
        } else {
            sent++;  
        }
    }

    USART_ITConfig(usart_handle, USART_IT_TC, SET);

    return sent;
}

void USART2_IRQHandler(void) {
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
        uint8_t data = USART2->DR;
        bool removeSuccess = 1;
        if(data == '\r' || data == '\n'){
            usbLineReceived = true;
        }
        // Check if it was a backspace.
        // '\b' for minicmom
        // '\177' for putty
        else if(data != '\b' && data != '\177') {
            rxfifo_put(&usbRxFifo, data);
        }
        // Sweet, just a "regular" key. Put it into the fifo
        // Doesn't matter if it fails. If it fails, then the data gets thrown away
        // and the easiest solution for this is to increase RX_SIZE
        else {
            char junk;
            // Delete the last entry!
            removeSuccess = rxfifo_popback(&usbRxFifo, &junk); 

            USART_SendData(USART2, 0x7F);   // TODO: Not sure if the backspace works. Need to test
        }
        if(removeSuccess) {
            USART2->DR = data;
        }
    }
    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
        // If getting data from fifo fails i.e. the tx fifo is empty, then turn off the TX interrupt
        if(!txfifo_get(&usbTxFifo, (char*)&(USART2->DR))) {
            USART_ITConfig(USART2, USART_IT_TC, RESET); // Turn off the interrupt
        }
    }
    if(USART_GetITStatus(USART2, USART_IT_ORE) != RESET);
}