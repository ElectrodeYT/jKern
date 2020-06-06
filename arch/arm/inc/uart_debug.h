// jKern arch/arm/uart_debug.h
// Simple, interruptless, MMU-less deubg routines
#ifndef UART_DEBUG
#define UART_DEBUG

void uart_put(char c);
void uart_puts(char cs[]);

void uart_putint(int i, int base);

#endif
