// jKern arch/arm/kernel_main.c
// ARM Kernel Entrypoint
#include <peripherals.h>
#include <uart_debug.h>
#include <memory.h>

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
    // Create Memory stuff
    clear_allocated_page_frames();
    int kernel_heap_page = allocate_page_frame();
    int test_page_1 = allocate_page_frame();
    int test_page_2 = allocate_page_frame();
    int test_page_3 = allocate_page_frame();
    unallocate_page_frame(test_page_2);
    int test_page_4 = allocate_page_frame(); // should be the same as test_page_2
}
