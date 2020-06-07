// jKern arch/arm/kernel_main.c
// ARM Kernel Entrypoint
#include <peripherals.h>
#include <uart_debug.h>
#include <memory.h>
#include <liballoc.h>
#include <memory_functions.h>
#include <paging.h>

extern int mmu_test_support();
extern void setup_irq();



// Use C linkage for this
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
    do {
        // TODO: unallocate all previously allocated frames
        translation_table = allocate_page_frame(1);
    } while(translation_table & 0x7FFF);
    uint64_t ttbr1 = (uint64_t)translation_table;
    // zero the translation table
    memset((void*)translation_table, MEMORY_PAGE_SIZE, 0);
    init_kernel_translation_table(translation_table);
    // set the ttbr0 register to the address of the translation table
    mmu_set_ttbr1(ttbr1);
    enable_mmu();
}
