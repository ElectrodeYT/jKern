// jKern arch/arm/asm/abort.s
// Abort stub
.global abort

abort:
  .loop:
  b .loop
