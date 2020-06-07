// jKern arch/arm/libmalloc_hooks.c
// Hooks for libAlloc
#include <stdint.h>
#include <stddef.h>
//#include <liballoc.h>
#include <memory.h>

int liballoc_lock() {
    // stub
    return 0;
}

int liballoc_unlock() {
    // stub
    return 0;
}
void* liballoc_alloc(size_t pages) {
    return (void*)allocate_page_frame(pages);
}

int liballoc_free(void* ptr, size_t pages) {
    return unallocate_page_frame((uint32_t)ptr, pages);
}
