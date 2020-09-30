// jKern base/inc/processes.h
// Header for the process manager

#ifndef PROCESSES_H
#define PROCESSES_H

// Process linked list
// This must be, and remain, CPU independent code!

struct AllocatedPages {
    uint32_t page_phys;
    uint32_t page_virt;
    struct AllocatedPages* next;
};

struct Processes {
    struct Registers* regs;
    uint32_t ttbr0_data; // Address
    uint32_t current_pc;
    uint32_t code_page_allocated;
    uint32_t code_page_begin;
    struct Processes* next;
    struct AllocatedPages* pages;
};

void setup_ttbr0();

void set_usermode_registers(struct Registers* regs);
void save_usermode_registers(struct Registers* regs);
__attribute__((noreturn))
void call_usermode(uint32_t pc);

int spawn_service(uint32_t begin, uint32_t size);
// for debugging scheduler: add predefined service
//int spawn_service();
int schedule();
// switch context
int switch_context(int previous_pc);

#endif
