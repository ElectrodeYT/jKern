// jKern arch/arm/kernel_main.c
// ARM Kernel Entrypoint
#include <peripherals.h>
#include <uart_debug.h>
#include <memory.h>
#include <liballoc.h>
#include <memory_functions.h>
#include <paging.h>
#include <processes.h>

extern int mmu_test_support();
extern void setup_irq();

// Test service
void service_test();
int service_test_size;

extern void kernel_main() {
    setup_irq();
    // Check MMU Support
    uart_puts_mmuless("Kernel_Main() Boot\n\r");
    if(mmu_test_support() == 0) {
      uart_puts_mmuless("No v6/v7 MMU Detected! Halted\n\r");
      return;
    }
    // Zero allocated pages boolean array
    clear_allocated_page_frames();


    // Create Memory stuff
    // We set TTBR1 to the upper two gigs for the kernel
    // and TTBR0 to the rest
    mmu_set_ttbcr(1);
    // Setup kernel page table
    init_kernel_translation_table();
    // Enable MMU
    enable_mmu();

    // MMU is now enabled
    setup_ttbr0();
    
    spawn_service((uint32_t)service_test, service_test_size);
    while(1) {
        schedule();
    }
}
