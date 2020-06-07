// jKern ARM Entrypoint

.global os_boot
os_boot:
  // Set non-secure bit
  mrc p15, 0, r0, c1, c1
  orr r0, #0x1
  mcr p15, 0, r0, c1, c1
  ldr sp, =STACK_TOP
  bl kernel_main
  swi 1
  swi 2
  swi 3
  swi 4
  hang:
  b hang

.size os_boot, . - os_boot
