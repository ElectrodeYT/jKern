// jKern arch/arm/inc/memory.h
// Memory Manager Header
#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdbool.h>

#define KERNEL_END 0x800F0000
// Free page frame list
#define MEMORY_FREE_PAGE_FRAMES KERNEL_END
#define MEMORY_PAGE_SIZE 4096
#define MEMORY_PAGE_LIST_AVAILBLE_PAGE_FRAMES ((128 * 1024 * 1024) / MEMORY_PAGE_SIZE)
#define MEMORY_FREE_PAGE_FRAMES_SIZE (MEMORY_PAGE_LIST_AVAILBLE_PAGE_FRAMES / 8)
#define MEMORY_FREE_PAGE_FRAMES_END (MEMORY_FREE_PAGE_FRAMES + MEMORY_FREE_PAGE_FRAMES_SIZE)
// memory page 0 is the page frame list
#define MEMORY_BEGIN (KERNEL_END + 4096)

bool clear_allocated_page_frames();

// Find the next free page table
// Returns: address of allocated page frame
uint32_t allocate_page_frame(int pages);
uint32_t allocate_page_frame_aligned(int pages, uint32_t aligment);
// Unallocate page frame
bool unallocate_page_frame(uint32_t page_frame, int pages);

extern void mmu_set_ttbcr(uint32_t ttbcr);
extern void mmu_set_ttbr0(uint64_t ttbr0);
extern void mmu_set_ttbr1(uint64_t ttbr1);
extern void enable_mmu();

void add_translation_descriptor_block(uint32_t translation_table, unsigned char id, uint32_t output_address);
void add_translation_descriptor_page(uint32_t translation_table, unsigned char id, uint32_t page_address);


// nice lib for kmalloc and kfree
#include <liballoc.h>

#endif
