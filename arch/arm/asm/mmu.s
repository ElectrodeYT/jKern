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
  cmp r0, #0b101
  bne .mmu_test_support_return_fail
  // MMU Supported
  mov r0, #1
  mov r1, #1
  bx lr
  .mmu_test_support_return_fail:
  mov r0, #0
  mov r1, #0
  bx lr

.global mmu_set_ttbr0
mmu_set_ttbr0:
  // Bodge:
  // For some reason r1 is 1 but we want it 0
  mov r1, #0
  mcrr p15, 0, r0, r1, c2
  // perform prefetch flush
  mcr p15, 0, r1, c7, c5, 4
  bx lr

.global mmu_set_ttbr1
mmu_set_ttbr1:
  // Bodge:
  // For some reason r1 is 1 but we want it 0
  mov r1, #0
  mcrr p15, 1, r0, r1, c2
  // perform prefetch flush
  mcr p15, 0, r1, c7, c5, 4
  bx lr



.global mmu_set_ttbcr
mmu_set_ttbcr:
  mcr p15, 0, r0, c2, c0, 2
  bx lr

.global enable_mmu
enable_mmu:
  // set DACR to all ones
  mov r0, #0xFFFFFFFF
  mcr p15, 0, r0, c3, c0, 0
  // disable caches
  mrc p15, 0, r0, c1, c0
  bic r0, #0x4
  bic r0, #0x1000
  mcr p15, 0, r0, c1, c0
  // enable mmu
  mrc p15, 0, r0, c1, c0
  orr r0, #0x1 // M bit
  //orr r0, #0x2000000
  orr r0, #0x800000
  mov r1, #0 // prefetch flush command
  // actually enable mmu
  mcr p15, 0, r0, c1, c0, 0
  // perform prefetch flush
  mcr p15, 0, r1, c7, c5, 4
  bx lr
