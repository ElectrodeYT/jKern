// jKern arch/arm/paging.h
// Paging code

#include <stdint.h>
#include <memory.h>
#include <memory_functions.h>
#include <paging.h>

uint32_t ttbr1 = 0;

void init_kernel_translation_table() {
    // Allocate space
    ttbr1 = allocate_page_frame_aligned(4, 0x7FFF);
    uint64_t ttbr1_func = (uint64_t)ttbr1;

    // zero the translation table
    memset((void*)ttbr1, MEMORY_PAGE_SIZE * 4, 0);

    // Identity map the top two gigs
    int adr = 0x80000000;
    for(int i = 0; i < (0xFFF - 0x800); i++) {
        add_translation_descriptor_block(ttbr1, ((adr & 0xFFF00000) >> 20), adr);
        adr += 0x100000;
    }

    // Set TTBR1 mmu register
    mmu_set_ttbr1(ttbr1_func);
}

// Creates a Translation table that can have addresses mapped to it
// Realativley easily
uint32_t create_user_translation_table() {
    // Because TTBCR.N == 1, the ttbr0 table is 8k big
    uint32_t translation_table_for_ttbr0 = allocate_page_frame(2); // 1 page = 4k
    // Zero the page
    memset((void*)translation_table_for_ttbr0, 4096 * 2, 0);
    return translation_table_for_ttbr0;
}

// Map a physical address page to a virtual address
void map_address(uint32_t translation_table, uint32_t phys, uint32_t virt) {
    // Calculate indexes into the translation table
    uint32_t first_level = (virt & 0xFFF00000) >> 20;
    uint32_t second_level = (virt & 0xFF000) >> 12;

    // Read entry in the first level table
    uint32_t first_entry = *(volatile uint32_t*)(translation_table + (first_level * 4));
    if(first_entry == 0) {
        // TODO: not waste 3k / page
        *(volatile uint32_t*)(translation_table + (first_level * 4)) = allocate_page_frame(1) | 1;
        first_entry = *(volatile uint32_t*)(translation_table + (first_level * 4));
    }
    // Write entry in the second level table
    *(volatile uint32_t*)((first_entry & 0xFFFFFC00) + (second_level * 4)) = (phys & 0xFFFFF000) | 0x32;
}

bool destroy_translation_table() {
    // TODO: this

    return true;
}

// Add a 1MB section to a translation table
void add_translation_descriptor_block(uint32_t translation_table, uint32_t id, uint32_t output_address) {
    *(volatile uint32_t*)(translation_table + (id * 4)) = (output_address & 0xFFF00000) | 0xC02;
}
