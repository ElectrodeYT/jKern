ENTRY(start)

SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x80000;
    /* For AArch64, use . = 0x80000; */
    /* Make sure our entry stub goes first */
    .stub   : { *(.text.boot) }
    /* c++ constructors and destructors */
    .ctor   : { KEEP(*(SORT(.ctors.*))) }
    .text   : { *(.text) }
    .rodata : { *(.rodata) }
    .data   : { *(.data) }

    /* we define these in order to clear out bss */
    __bss_start = .;
    .bss    : { *(.bss COMMON) }
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
    STACK_BASE = .;
    . += 0x10000;
    STACK_TOP = .;
}
