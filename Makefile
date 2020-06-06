ARCH := arm

C := $(ARCH)-none-eabi-gcc
AS := $(ARCH)-none-eabi-as

C_ARGS := -mcpu=cortex-a15 -fpic -ffreestanding -fno-exceptions -Iarch/$(ARCH)/inc -Ibase/inc -g -Wall
AS_ARGS := -g

assembly := $(wildcard arch/$(ARCH)/asm/*.s)
c := $(wildcard arch/$(ARCH)/*.c) $(wildcard base/*.c)

assembly_o := $(filter-out crtn.o,$(filter-out crti.o,$(assembly:%.s=%.o)))
c_o := $(c:%.c=%.o)

linker_file := arch/$(ARCH)/link.ld

kernel := jKern.elf
boot_image := jKern.img
objects := $(assembly_o) $(c_o)

.PHONY: all create-image link qemu

%.o: %.s
	@echo "[ASM]		" $< $@
	@$(AS) -c $< -o $@ $(AS_ARGS)

%.o: %.c
	@echo "[C]		" $< $@
	@$(C) -c $< -o $@ $(C_ARGS)

link $(kernel): $(objects)
	@echo [LINK] $(kernel)
	@$(ARCH)-none-eabi-gcc -T $(linker_file) -o $(kernel) -ffreestanding -nostdlib -lgcc -nostdlib $(objects)


create-image $(boot_image): $(kernel)
	@echo [OBJCOPY] $(boot_image)
	@$(ARCH)-none-eabi-objcopy $(kernel) -O binary $(boot_image)

all: $(boot_image)

qemu: $(kernel)
	@#qemu-system-arm -s -m 256 -M vexpress-a15 -cpu cortex-a15 -serial stdio -kernel $(kernel)
	@gdb-multiarch -x gdb.gdb -tui
clean:
	@rm $(objects) $(kernel) $(boot_image) 2> /dev/null; true
