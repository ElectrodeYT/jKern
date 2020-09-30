// jKern arch/arm/asm/irq.s
// Interrupt handeling code

.global setup_irq
setup_irq:
  // Set IRQ Vector table location
  push {r0-r10}
  ldr r0, =irq_vector_table
  mcr p15, 0, r0, c12, c0, 0
  // Set Enable bit
  ldr r0, gic_cpu_ctlr
  ldr r1, gic_cpu_ctlr_enable_val
  str r1, [r0]
  pop {r0-r10}
  bx lr

gic_cpu_ctlr:
.int 0x7ff04000
gic_cpu_ctlr_enable_val:
.int 1

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
b irq_handler
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
  str sp, software_handler_sp_store
  ldr sp, =irq_stack_top
  push {r0-r12, lr}
  
  // Read usermode registers from system mode

  msr cpsr, #0x1f
  // Write into variables from system mode
  str r0, software_handler_r0_write
  str r1, software_handler_r1_write
  str r2, software_handler_r2_write
  msr cpsr, #0x13
  // Read from variables in supervisor mode
  ldr r0, software_handler_r0_write
  ldr r1, software_handler_r1_write
  ldr r2, software_handler_r2_write

  bl software_interrupt_c_handler
  
  // Write r0 to variable
  str r0, software_handler_r0_write
  // Restore supervisor mode registers
  pop {r0-r12, lr}
  // Switch to system mode
  msr cpsr, #0x1f
  // Load user mode r0
  ldr r0, software_handler_r0_write
  // Switch back to supervisor mode
  msr cpsr, #0x13
  // Restore stack pointer
  ldr sp, software_handler_sp_store

  // Return
  subs pc, lr, #0

software_handler_sp_store:
.int 0
software_handler_r0_write:
.int 0
software_handler_r1_write:
.int 0
software_handler_r2_write:
.int 0

.global irq_handler
irq_handler:
  ldr sp, =irq_stack
  push {r0-r12, lr}
  // Load Interrupt number
  ldr r1, gic_int_ack
  ldr r0, [r1]
  ldr r2, gic_int_mask
  and r0, r2

  
  pop {r0-r12, lr}
  b irq_handler

gic_int_ack:
.int 0x7ff0400C
gic_int_mask:
.int 0x3FF

// Stack used by interrupt routines
irq_stack:
  .skip 1024, 0
irq_stack_top:
