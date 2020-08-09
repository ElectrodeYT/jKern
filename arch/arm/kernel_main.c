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
    uart_puts("Kernel_Main() Boot\n\r");
    if(mmu_test_support() == 0) {
      uart_puts("No MMU Detected! Halted\n\r");
      return;
    }

    clear_allocated_page_frames();
    // Create Memory stuff
    // We set TTBR1 to the upper two gigs for the kernel
    // and TTBR0 to the rest
    mmu_set_ttbcr(0x80000001);
    // really shitty code to get a correctly aligned page frame allocated for this
    unsigned int translation_table;

    translation_table = allocate_page_frame_aligned(1, 0x7FFF);
    uint64_t ttbr1 = (uint64_t)translation_table;
    // zero the translation table
    memset((void*)translation_table, MEMORY_PAGE_SIZE, 0);
    init_kernel_translation_table(translation_table);
    // set the ttbr1 register to the address of the translation table
    mmu_set_ttbr1(ttbr1);
    enable_mmu();
    // MMU is now enabled
    // Setup space for the ttbr0 translation table register

    //translation_table = allocate_page_frame_aligned(1, 0x7FFF);
    //uint64_t ttbr0 = (uint64_t)translation_table;
    //mmu_set_ttbr0(ttbr0);

    setup_ttbr0();
    spawn_service(service_test, service_test_size);
    while(1) {
        schedule();
    }
}
