// jKern arch/asm/process_hw.s
// Process assembler code

.global set_usermode_registers
set_usermode_registers:
  // Save registers
  push {r0-r11}
  // mrs r0, cpsr_all
  nop
  // Switch to arm system mode

  msr cpsr, #0x1f
  ldmia r0, {r0-r14}
  msr cpsr, #0x13
  nop
  pop {r0-r11}
  bx lr

.global call_usermode
call_usermode:
  // No return is expected
  str r0, fuck_this_shit_im_out
  // Switch to user mode
  msr cpsr, #0x10
  // Branch to user pc
  ldr r15, fuck_this_shit_im_out
  bx lr // should never get here lol

fuck_this_shit_im_out:
.int 0
