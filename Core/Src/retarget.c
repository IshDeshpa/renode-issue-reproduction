/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "uart.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

int _write(int fd, char *buffer, unsigned int len) {
    if(buffer != NULL) {
        uart_write(buffer, len);
    }
    return len;
}

int _read(int const fd, char *buffer, unsigned const len) {
    if(buffer != NULL) {
        
    }
    return 1;
}

int _close(int file)
{
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

// These functions are included to remove GCC warnings when compiling and are not necessary
int _isatty(int fd) {
    return 0;
}

int _fstat(int fd) {
    return -1;
}




