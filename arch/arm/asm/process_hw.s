// jKern arch/asm/process_hw.s
// Process assembler code

// used for the copy of the registers
.extern memcpy


.global set_usermode_registers
set_usermode_registers:
  push {r0-r12, lr}
  // Copy register struct to the variables bit
  // r0 is already source
  ldr r1, =user_mode_registers_variables
  mov r2, #50
  bl memcpy
  // Switch to system mode
  msr cpsr, #0x1f
  ldr r0, user_mode_r0
  ldr r1, user_mode_r0
  ldr r2, user_mode_r0
  ldr r3, user_mode_r0
  ldr r4, user_mode_r0
  ldr r5, user_mode_r0
  ldr r6, user_mode_r0
  ldr r7, user_mode_r0
  ldr r8, user_mode_r0
  ldr r9, user_mode_r0
  ldr r10, user_mode_r0
  ldr r11, user_mode_r0
  ldr r12, user_mode_r0
  ldr sp, user_mode_r0
  ldr lr, user_mode_r0
  // Switch back to supervisor mode
  msr cpsr, #0x13
  // Return
  pop {r0-r12, lr}
  bx lr

.global save_usermode_registers
save_usermode_registers:
  push {r0-r12, lr}
  // Switch to system mode
  msr cpsr, #0x1f
  str r0, user_mode_r0
  str r1, user_mode_r0
  str r2, user_mode_r0
  str r3, user_mode_r0
  str r4, user_mode_r0
  str r5, user_mode_r0
  str r6, user_mode_r0
  str r7, user_mode_r0
  str r8, user_mode_r0
  str r9, user_mode_r0
  str r10, user_mode_r0
  str r11, user_mode_r0
  str r12, user_mode_r0
  str sp, user_mode_r0
  str lr, user_mode_r0
  // Switch back to supervisor mode
  msr cpsr, #0x13
  // Setup memcpy arguments
  mov r1, r0
  ldr r0, =user_mode_registers_variables
  mov r2, #50
  bl memcpy // Execute memcpy
  pop {r0-r12, lr}
  bx lr

user_mode_registers_variables:
user_mode_r0:
.int 0 // r0
user_mode_r1:
.int 0 // r1
user_mode_r2:
.int 0 // r2
user_mode_r3:
.int 0 // r3
user_mode_r4:
.int 0 // r4
user_mode_r5:
.int 0 // r5
user_mode_r6:
.int 0 // r6
user_mode_r7:
.int 0 // r7
user_mode_r8:
.int 0 // r8
user_mode_r9:
.int 0 // r9
user_mode_r10:
.int 0 // r10
user_mode_r11:
.int 0 // r11
user_mode_r12:
.int 0 // r12
user_mode_sp:
.int 0 // sp
user_mode_lr:
.int 0 // lr
user_mode_pc:
.int 0 // pc


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
