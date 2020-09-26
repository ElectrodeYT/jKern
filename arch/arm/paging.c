// jKern arch/arm/paging.h
// Paging code

#include <stdint.h>
#include <memory.h>
#include <memory_functions.h>
#include <paging.h>

uint32_t ttbr1 = 0;

void init_kernel_translation_table() {
    // Allocate space
    ttbr1 = allocate_page_frame_aligned(1, 0x7FFF);
    uint64_t ttbr1_func = (uint64_t)ttbr1;

    // zero the translation table
    memset((void*)ttbr1, MEMORY_PAGE_SIZE, 0);
    // For now we just identity map the top two gigs
    add_translation_descriptor_block(ttbr1, 0b10, 0x80000000);
    add_translation_descriptor_block(ttbr1, 0b11, 0xC0000000);
    // Set TTBR1 mmu register
    mmu_set_ttbr1(ttbr1_func);
}

// Creates a Translation table that can have addresses mapped to it
// Realativley easily
uint32_t create_user_translation_table() {
    // Because of ARMs excellent design of the first translation table, it is easier and less "expensive" to allocate
    // the first level table in the kernel heap

    // While that table technically has 4 entries, our configuration of ttbr0/ttbr1 means that the last two will never be walked so
    // we can safely not allocate them
    uint32_t translation_table_for_ttbr0 = (uint32_t)kmalloc(8 * 2); // two entries
    memset((void*)translation_table_for_ttbr0, 8 * 2, 0); // zero those two entries out
    // We make second-level pages for this already, lots easier to do it now then later
    uint32_t second_level_table1 = allocate_page_frame(1);
    uint32_t second_level_table2 = allocate_page_frame(1);
    // Zero both tables
    memset((void*)second_level_table1, MEMORY_PAGE_SIZE, 0);
    memset((void*)second_level_table2, MEMORY_PAGE_SIZE, 0);
    // Add the tables to the translation table
    add_translation_descriptor_page(translation_table_for_ttbr0, 0b00, second_level_table1);
    add_translation_descriptor_page(translation_table_for_ttbr0, 0b01, second_level_table2);
    return translation_table_for_ttbr0;
}

// Map a physical address page to a virtual address
void map_address(uint32_t translation_table, uint32_t phys, uint32_t virt) {
    // Calculate indexes into the translation table
    int first_level_table_index = (virt & 0xC0000000) >> 30;
    int second_level_table_index = (virt & 0x3FE00000) >> 21;
    int third_level_table_index = (virt & 0x1FF000) >> 12;

    // Get second level table
    uint32_t table_two_base = *(volatile uint32_t*)(translation_table + (first_level_table_index  * 8)) & 0xFFFFF000;
    if(*(volatile uint32_t*)(table_two_base + (second_level_table_index * 8)) == 0) {
        *(volatile uint32_t*)(table_two_base + (second_level_table_index * 8)) = allocate_page_frame(1) | 0x43;
        *(volatile uint32_t*)(table_two_base + (second_level_table_index * 8) + 4) = 0;
    }

    // Get third level index
    uint32_t table_three_base = *(volatile uint32_t*)(table_two_base + (second_level_table_index * 8)) & 0xFFFFF000;
    *(volatile uint32_t*)(table_three_base + (third_level_table_index * 8)) = (phys & 0xFFFFF000) | 0x4C3;
    *(volatile uint32_t*)(table_three_base + (third_level_table_index * 8) + 4) = 0;
}

bool destroy_translation_table() {
    // TODO: this

    return true;
}

// Add a 1GB block to a translation table
void add_translation_descriptor_block(uint32_t translation_table, unsigned char id, uint32_t output_address) {
    *(volatile uint32_t*)(translation_table + (id * 8)) = (output_address & 0xFFFFF000) | 0x441;
    *(volatile uint32_t*)(translation_table + (id * 8) + 4) = 0;
}

// Add a pointer to a second level translation table to a first level table
void add_translation_descriptor_page(uint32_t translation_table, unsigned char id, uint32_t page_address) {
    *(volatile uint32_t*)(translation_table + (id * 8)) = (page_address & 0xFFFFF000) | 0x443;
    *(volatile uint32_t*)(translation_table + (id * 8) + 4) = 0;
}
