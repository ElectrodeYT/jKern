// jKern arch/arm/uart_debug.h
// Simple, interruptless, MMU-less debug routines
#ifndef UART_DEBUG
#define UART_DEBUG

void uart_put(char c);
void uart_puts(char cs[]);

void uart_put_mmuless(char c);
void uart_puts_mmuless(char cs[]);


void uart_putint(int i, int base);

#endif
