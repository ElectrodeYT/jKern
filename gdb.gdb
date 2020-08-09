file jKern.elf
target remote | qemu-system-arm -M vexpress-a15 -cpu cortex-a15 -kernel jKern.elf -S -gdb stdio -m 2048
set disassemble-next-line on
