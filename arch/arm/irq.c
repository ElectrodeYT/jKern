// jKern arch/arm/irq.c
// Interrupt handler code

#include <uart_debug.h>
#include <stdint.h>

// SWI interrupt
uint32_t software_interrupt_c_handler(uint32_t r0, uint32_t r1) {
    // decode systemcall
    switch(r0) {
        case 0:
            uart_put((char)r1);
            break;

        default:
            break;
    }

    return 1;
}
