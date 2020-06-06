// jKern arch/arm/peripherals.hpp
// ARM peripherals

#ifndef PERIPHERALS_HPP
#define PERIPHERALS_HPP
#include <stdint.h>

#define UART0_BASE 0x1c090000
#define GIC_BASE 0x2C000000



#define WRITE_REG(A, B) *(volatile uint32_t*)A = B;

#endif
