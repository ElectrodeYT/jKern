// jKern arch/arm/paging.h
// Paging code

#include <stdint.h>
#include <memory.h>
#include <memory_functions.h>
#include <paging.h>

void init_kernel_translation_table(uint32_t translation_table) {
    // For now we just identity map the top two gigs
    add_translation_descriptor_block(translation_table, 0b10, 0x80000000);
    add_translation_descriptor_block(translation_table, 0b11, 0xC0000000);
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
    int first_level_table_index = (virt & 0xC0000000) >> 30;
    int second_level_table_index = (virt & 0x3FE00000) >> 21;
    int third_level_table_index = (virt & 0x1FF000) >> 12;
    // Get first table index address
    uint32_t* table_one = (uint32_t*)(translation_table);
    table_one += first_level_table_index * 8;
    uint32_t table_two_base = table_one[0] & 0xFFFFF000;
    uint32_t* table_two = (uint32_t*)(table_two_base);
    table_two += second_level_table_index * 8;
    if(table_two[0] == 0) {
        table_two[0] = allocate_page_frame(1) | 0xC3;
    }
    uint32_t table_three_base = table_two[0] & 0xFFFFF000;
    uint32_t* table_three = (uint32_t*)(table_three_base);
    table_three += third_level_table_index * 8;
    table_three[0] = (phys & 0xFFFFF000) | 0x443;

}

bool destroy_translation_table() {
    // TODO: this

    return true;
}
