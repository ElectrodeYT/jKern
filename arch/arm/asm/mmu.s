// jKern arch/arm/asm/mmu.s
// MMU Assembly control code

.global mmu_test_support
// Check if the mmu is supported
mmu_test_support:
  mrc p15, 0, r0, c0, c1, 4
  mov r1, #0b1111
  and r0, r1
  cmp r0, #0
  beq .mmu_test_support_return_fail
  // MMU Supported
  mov r0, #1
  mov r1, #1
  bx lr
  .mmu_test_support_return_fail:
  mov r0, #0
  mov r1, #0
  bx lr
