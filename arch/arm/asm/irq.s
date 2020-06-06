// jKern arch/arm/asm/irq.s
// Interrupt handeling code

.global setup_irq
setup_irq:
  // Set IRQ Vector table location
  ldr r0, =irq_vector_table
  mcr p15, 0, r0, c12, c0, 0
  bx lr

.align 0b1000

.global irq_vector_table
irq_vector_table:
unused_vector:
b unused_vector // Unused interrupt vector
undefined_instruction:
b undefined_instruction // Undefined Instruction
software_interrupt:
b software_handler
prefetch_abort:
b prefetch_abort
data_abort:
b data_abort
not_used_2:
b not_used_2



.global software_handler
software_handler:
  b software_handler

.global irq_handler
irq_handler:
  b irq_handler
