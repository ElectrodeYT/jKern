// jKern arch/arm/uart-debug.cpp
// UART debug code

#include <uart_debug.h>
#include <peripherals.h>

void uart_put(char c) {
  WRITE_REG(UART0_BASE, c);
}

void uart_puts(char cs[]) {
  for(int i = 0; cs[i] != 0; i++) {
    uart_put(cs[i]);
  }
}
