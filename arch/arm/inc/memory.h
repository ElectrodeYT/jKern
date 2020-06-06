// jKern arch/arm/inc/memory.h
// Memory Manager Header
#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdbool.h>

#define KERNEL_END 0x800F0000
// Free page frame list
#define MEMORY_FREE_PAGE_FRAMES KERNEL_END
#define MEMORY_PAGE_LIST_AVAILBLE_PAGE_FRAMES ((128 * 1024 * 1024) / 4096)
#define MEMORY_FREE_PAGE_FRAMES_SIZE (MEMORY_PAGE_LIST_AVAILBLE_PAGE_FRAMES / 8)
#define MEMORY_FREE_PAGE_FRAMES_END (MEMORY_FREE_PAGE_FRAMES + MEMORY_FREE_PAGE_FRAMES_SIZE)

#define MEMORY_BEGIN MEMORY_FREE_PAGE_FRAMES_END

bool clear_allocated_page_frames();

// Find the next free page table
// Returns: address of allocated page frame
uint32_t allocate_page_frame();
// Unallocate page frame
bool unallocate_page_frame();

#endif
