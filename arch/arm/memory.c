// jKern arch/arm/memory.c
// Memory Manager code
#include <memory.h>
#include <uart_debug.h>
#include <stdbool.h>
#include <abort.h>
#include <memory_functions.h>

uint32_t allocate_page_frame() {
    int page = 0; // The page being inspected, in bits
    unsigned char* page_frame_list = (unsigned char*)MEMORY_FREE_PAGE_FRAMES;
    for(; page < MEMORY_PAGE_LIST_AVAILBLE_PAGE_FRAMES; page++) {
        int byte = page / 8; // byte to look at
        int bit = page % 8; // bit to look at
        int is_page_free = (page_frame_list[byte] >> bit) & 0x01;
        if(is_page_free == 0) {
            // we have found a page frame
            page_frame_list[byte] = page_frame_list[byte] | (0x01 << bit);
            return (MEMORY_BEGIN + (4096 * page));
        }
    }
    return 0;
}

bool unallocate_page_frame(uint32_t page_frame) {
    // Get page id from address
    int page = (page_frame - MEMORY_BEGIN) / 4096;
    int byte = page / 8;
    int bit = page % 8;
    unsigned char* page_frame_list = (unsigned char*)MEMORY_FREE_PAGE_FRAMES;
    // Clear the bit for that page
    page_frame_list[byte] = page_frame_list[byte] ^ (0x01 << bit);
    return true;
}

bool clear_allocated_page_frames() {
    memset((void*)(MEMORY_FREE_PAGE_FRAMES), MEMORY_FREE_PAGE_FRAMES_SIZE, 0);
    return true;
}
