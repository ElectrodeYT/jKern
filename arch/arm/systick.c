#include <peripherals.h>
#include <systick.h>

int setup_systick() {
    // Program reload value
    *(volatile uint32_t*)(SYSTICK_RVR) = 0x1000;
    // Clear counter register
    *(volatile uint32_t*)(SYSTICK_CVR) = 0;
    return 0;
}

int enable_systick() {
    // Clear counter register
    *(volatile uint32_t*)(SYSTICK_CVR) = 0;
    // Program reload value
    *(volatile uint32_t*)(SYSTICK_RVR) = 0x1000;
    // Enable systick
    *(volatile uint32_t*)(SYSTICK_CSR) = 0b111;
    return 0;
}

int disable_systick() {
    // Disable systick
    *(volatile uint32_t*)(SYSTICK_CSR) = 0;
    // Clear counter register
    *(volatile uint32_t*)(SYSTICK_CVR) = 0;
    return 0;
}