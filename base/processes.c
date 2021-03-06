// jKern base/processes.c
// Process Manager
#include <stdint.h>
#include <stddef.h>
#include <processes.h>
#include <processes_hw.h>
#include <memory.h>
#include <memory_functions.h>
#include <paging.h>
#include <peripherals.h>

struct Processes* list = NULL;
// We use 1 memory area for ttbr0, we just copy a new one to it when switching a process
// also, fuck arm
int ttbr0 = 0;
int process = 0;

// Add page to allocated pages list
void add_allocated_pages_page(struct AllocatedPages* pages, uint32_t page_phys, uint32_t page_virt) {
    struct AllocatedPages* curr = pages;
    while(curr->next != NULL) {
        curr = curr->next;
    }
    // curr->next == NULL
    curr->next = kmalloc(sizeof(struct AllocatedPages));
    curr->next->page_phys = page_phys;
    curr->next->page_virt = page_virt;
}

void setup_ttbr0() {
    // Allocate alligned page frame
    uint32_t translation_table = allocate_page_frame_aligned(2, 0x7FFF);
    ttbr0 = translation_table;
    memset((void*)ttbr0, 4096 * 2, 0);
    uint64_t ttbr0_func = (uint64_t)translation_table;
    map_address(ttbr0, UART0_BASE, UART0_VIRT_BASE);
    map_address(ttbr0, SYSTICK_BASE, SYSTICK_VIRT_BASE);
    map_address(ttbr0, GIC_DISTRIBUTOR_BASE, GIC_DISTRIBUTOR_VIRT_BASE);
    map_address(ttbr0, GIC_CPU_BASE, GIC_CPU_VIRT_BASE);
    
    mmu_set_ttbr0(ttbr0_func);
}

uint32_t copy_process_to_page_aligned_area(void* data, uint32_t sz, uint32_t* pages_allocated) {
    // Get pages for this shit
    // This should work as 1 simple calculation but the last one i did didnt work but this does so fuck it
    int page_count = 0;
    int sz_shit = sz;
    do {
        page_count++;
        sz_shit -= 4096;
    } while(sz_shit > 0);
    pages_allocated[0] = page_count;
    // Allocate pages
    uint32_t page_pointer = allocate_page_frame(page_count);
    // Copy data
    memcpy(data, (void*)page_pointer, sz);
    return page_pointer;
}

int spawn_service(uint32_t begin, uint32_t size) {
    struct Processes* pointer = list;
    int id = 0;
    // Create first process
    if(pointer == NULL) {
        list = kmalloc(sizeof(struct Processes));
        list->next = NULL;
        pointer = list;
    } else {
        // get last process pointer
        while(pointer->next != NULL) {
            pointer = pointer->next;
            id++;
        }
        // Create new process
        pointer->next = kmalloc(sizeof(struct Processes));
        pointer = pointer->next;
    }
    // Zero next
    pointer->next = NULL;
    // Stored Registers
    struct Registers* regs = kmalloc(sizeof(struct Registers));
    pointer->regs = regs;
    // Userspace Translation table
    uint32_t ttbr0_data = create_user_translation_table();
    pointer->ttbr0_data = ttbr0_data;
    // Get program start point
    uint32_t allocated_page_frames = 0;
    uint32_t program_start = copy_process_to_page_aligned_area((void*)begin, size, &allocated_page_frames);
    pointer->code_page_allocated = allocated_page_frames;

    pointer->code_page_begin = program_start;
    // Create pages structure
    pointer->pages = kmalloc(sizeof(struct AllocatedPages));
    // Map it all to mem 0
    for(int i = 0; i < pointer->code_page_allocated; i++) {
        map_address(pointer->ttbr0_data, (pointer->code_page_begin + (i * 4096)), i * 4096);
        add_allocated_pages_page(pointer->pages, (pointer->code_page_begin + (i * 4096)), i * 4096);
    }
    // Map IO
    map_address(pointer->ttbr0_data, UART0_BASE, UART0_VIRT_BASE);
    map_address(pointer->ttbr0_data, SYSTICK_BASE, SYSTICK_VIRT_BASE);
    pointer->current_pc = 0;
    return id;
}

// Run scheduler
int schedule() {
    // Get Process
    struct Processes* pointer = list;
    // Get the pointer to the process struct
    for(int i = 0; i < process; i++) {
        if(pointer == NULL) {
            // We have reached the end
            pointer = list;
            break;
        }
        pointer = pointer->next;
    }
    if(pointer == NULL) {
        return -1;
    }
    // Copy ttbr0 data
    memcpy((void*)(pointer->ttbr0_data), (void*)ttbr0, 4096 * 2);
    // Refresh MMU Tables
    mmu_set_ttbr0(ttbr0);
    // Set the user mode registers to the registers we want
    set_usermode_registers(pointer->regs);
    // Jump to the processes pc
    call_usermode(pointer->current_pc);
    return 0;
}

int switch_context(int previous_pc) {
    // Get Process
    struct Processes* pointer = list;
    // Get the pointer to the process struct
    for(int i = 0; i < process; i++) {
        if(pointer == NULL) {
            // We have reached the end
            pointer = list;
            process = 0;
            break;
        }
        pointer = pointer->next;
    }
    if(pointer == NULL) {
        return -1;
    }
    // Save registers
    save_usermode_registers(pointer->regs);
    // Save PC
    pointer->regs->pc = previous_pc;
    // Increment process
    process++;
    // Jump to new process
    schedule();
    return 0;
}