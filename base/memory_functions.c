// jKern base/memory_functions.c
// memcpy/memsz

#include <stdint.h>

void memset(void* mem, int sz, unsigned char filler) {
    unsigned char* mem_byte = (unsigned char*)mem;
    for(int i = 0; i < sz; i++) {
        mem_byte[i] = filler;
    }
}


void memcpy(void* src, void* dst, uint32_t sz) {
    unsigned char* src_byte = (unsigned char*)src;
    unsigned char* dst_byte = (unsigned char*)dst;
    for(uint32_t i = 0; i < sz; i++) {
        dst_byte[i] = src_byte[i];
    }
}
