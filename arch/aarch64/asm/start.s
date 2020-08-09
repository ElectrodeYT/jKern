// jKern arm/aarch64/start.s
// aarch64 entrypoint

// keep this at the start of the binary
.section ".text.boot"
.global start
.org 0x80000
start:
    // shut off other cores before starting
    // required for old firmware and qemu
    mrs x1, mpidr_el1 // read cpu id
    and x1, x1, #3
    cbz x1, core_0
not_core_0:
    wfe
    bne not_core_0 // hang it right here but

core_0:

    // set stack before our code
    ldr     x5, =kernel_stack_top
    mov     sp, x5

    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b

    // jump to C code, should not return
4:  bl      kernel_main
    // for failsafe, halt this core too
    b 4b


.section bss
kernel_stack:
  .skip 0x100, 0
kernel_stack_top:
