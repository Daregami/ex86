ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pic -mno-sse -mno-sse2 -mno-mmx -Wall -Iinclude -Wextra -MMD -MP

C_SRCS := $(shell find src -name '*.c')
ASM_SRCS := $(shell find src -name '*.asm')

C_OBJS := $(patsubst src/%.c, build/%.o, $(C_SRCS))
ASM_OBJS := $(patsubst src/%.asm, build/%.o, $(ASM_SRCS))
OBJS := $(ASM_OBJS) $(C_OBJS)

all: build/exomos.iso

build/%.o: src/%.asm
	mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.bin: $(OBJS) linker.ld
	$(LD) -m elf_i386 -T linker.ld -o $@ $(OBJS)

build/exomos.iso: build/kernel.bin src/boot/grub.cfg
	mkdir -p build/isodir/boot/grub
	cp build/kernel.bin build/isodir/boot/kernel.bin
	cp src/boot/grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ build/isodir

run: build/exomos.iso
	$(QEMU) -cdrom build/exomos.iso

clean:
	rm -rf build

.PHONY: all run clean