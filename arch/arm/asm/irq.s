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
b prefetch_abort_handler
data_abort:
b data_abort_handler
not_used_2:
b not_used_2
irq_interrupt:
b irq_interrupt
fiq_interrupt:
b fiq_interrupt


prefetch_abort_handler:
  subs pc, lr, #4 // litteraly just return lol

data_abort_handler:
  // setup the irq Stack
  ldr sp, =irq_stack_top
  // push unbanked registers
  push {r0-r12, lr}
  // calculate abort Instruction address
  subs r1, r14, #4
  // read dfsr register
  //mrs r2, dfsr
  .loop:
  b .loop

.global software_handler
.extern software_interrupt_c_handler
software_handler:
  // push all non banked registers
  // for all we know we could be called from supervisor mode that doesnt bank its own registers
  ldr sp, =irq_stack_top
  push {r0-r12, lr}
  bl software_interrupt_c_handler
  pop {r0-r12, lr}
  subs pc, lr, #0
.global irq_handler
irq_handler:
  b irq_handler


// Stack used by interrupt routines
irq_stack:
  .skip 1024, 0
irq_stack_top:
