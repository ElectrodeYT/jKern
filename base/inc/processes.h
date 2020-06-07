// jKern base/inc/processes.h
// Header for the process manager

#ifndef PROCESSES_H
#define PROCESSES_H

// Process linked list

struct Processes {
    uint32_t ttr0;
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t r13;
    uint32_t r14;
    uint32_t r15;

    struct Process* next;
};

int spawn_service(uint32_t jump);

#endif
