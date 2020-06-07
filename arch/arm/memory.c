// jKern arch/arm/memory.c
// Memory Manager code
#include <memory.h>
#include <uart_debug.h>
#include <stdbool.h>
#include <abort.h>
#include <memory_functions.h>

uint32_t allocate_page_frame(int pages) {
    int page = 1; // The page being inspected, in bits
    int pages_allocated = 0;
    unsigned char* page_frame_list = (unsigned char*)MEMORY_FREE_PAGE_FRAMES;
    for(; page < MEMORY_PAGE_LIST_AVAILBLE_PAGE_FRAMES; page++) {
        int byte = page / 8; // byte to look at
        int bit = page % 8; // bit to look at
        int is_page_free = (page_frame_list[byte] >> bit) & 0x01;
        if(is_page_free == 0) {
            // we have found a page frame
            pages_allocated++;
            if(pages_allocated == pages) {
                // we have allocated all requested (contiguous) pages
                page_frame_list[byte] = page_frame_list[byte] | (0x01 << bit);
                return (MEMORY_BEGIN + (4096 * page));
            }
        } else {
            pages_allocated = 0; // We need to reset this as they would no longer be contiguous
        }
    }
    return 0;
}

bool unallocate_page_frame(uint32_t page_frame, int amount) {
    // Get page id from address
    for(int i = 0; i < amount;  i++) {
        int page = ((page_frame - MEMORY_BEGIN) / 4096) + i;
        int byte = page / 8;
        int bit = page % 8;
        unsigned char* page_frame_list = (unsigned char*)MEMORY_FREE_PAGE_FRAMES;
        // Clear the bit for that page
        page_frame_list[byte] = page_frame_list[byte] ^ (0x01 << bit);
    }
    return true;
}

bool clear_allocated_page_frames() {
    memset((void*)(MEMORY_FREE_PAGE_FRAMES), MEMORY_FREE_PAGE_FRAMES_SIZE, 0);
    return true;
}

void add_translation_descriptor_block(uint32_t translation_table, unsigned char id, uint32_t output_address) {
    *(volatile uint32_t*)(translation_table + (id * 8) + 4) = (output_address & 0xFFFFF000) | 0b1000001;
    *(volatile uint32_t*)(translation_table + (id * 8)) = 0;
}
