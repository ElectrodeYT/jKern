// jKern ARM Entrypoint

.global os_boot
os_boot:
  // Check if in hypervisor mode
  mrs r0, cpsr_all
  ldr sp, =STACK_TOP
  bl kernel_main
  swi 1
  hang:
  b hang

.size os_boot, . - os_boot
