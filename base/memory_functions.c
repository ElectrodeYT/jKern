// jKern base/memory_functions.c
// memcpy/memsz

void memset(void* mem, int sz, unsigned char filler) {
    unsigned char* mem_byte = (unsigned char*)mem;
    for(int i = 0; i < sz; i++) {
        mem_byte[i] = filler;
    }
}
