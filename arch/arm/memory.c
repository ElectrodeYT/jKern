// jKern arch/arm/memory.c
// Memory Manager code
#include <memory.h>
#include <uart_debug.h>
#include <stdbool.h>
#include <abort.h>
#include <memory_functions.h>

uint32_t allocate_page_frame(int pages) {
    return allocate_page_frame_aligned(pages, 0);
}

uint32_t allocate_page_frame_aligned(int pages, uint32_t alignment) {
    int page = 1; // The page being inspected, in bits
    int pages_allocated = 0;
    int page_base_allocated = 0;
    unsigned char* page_frame_list = (unsigned char*)MEMORY_FREE_PAGE_FRAMES;
    for(; page < MEMORY_PAGE_LIST_AVAILBLE_PAGE_FRAMES; page++) {
        if(pages_allocated == 0 && ((MEMORY_BEGIN + (4096 * page)) & alignment)) {
            continue;
        }
        int byte = page / 8; // byte to look at
        int bit = page % 8; // bit to look a {
        int is_page_free = (page_frame_list[byte] >> bit) & 0x01;
        if(is_page_free == 0) {
            // we have found a page frame
            if(pages_allocated == 0) {
                // first page in a series
                page_base_allocated = page;
            }
            pages_allocated++;
            if(pages_allocated == pages) {
                // we have allocated all requested (contiguous) pages
                //page_frame_list[byte] = page_frame_list[byte] | (0x01 << bit);
                for(int pages_set = 0; pages_set <= page - page_base_allocated; pages_set++) {
                    // set a bit
                    int _byte = (page_base_allocated + pages_set) / 8;
                    int _bit = (page_base_allocated + pages_set) % 8;
                    page_frame_list[_byte] = page_frame_list[_byte] | (0x01 << _bit);
                }
                return (MEMORY_BEGIN + (4096 * page_base_allocated));
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