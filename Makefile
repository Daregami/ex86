ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pic -mno-sse -mno-sse2 -mno-mmx -Wall -Iinclude -Wextra -MMD -MP

all: bin/exomos.iso

bin/boot.o: src/boot/boot.asm
	mkdir -p bin
	$(ASM) -f elf32 src/boot/boot.asm -o bin/boot.o

bin/vga.o: src/drivers/vga.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c src/drivers/vga.c -o $@

bin/kernel.o: src/kernel/kernel.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c src/kernel/kernel.c -o $@

bin/kernel.bin: bin/kernel.o bin/boot.o bin/vga.o linker.ld
	$(LD) -m elf_i386 -T linker.ld -o $@ bin/boot.o bin/kernel.o bin/vga.o

bin/exomos.iso: bin/kernel.bin src/boot/grub.cfg
	mkdir -p bin/isodir/boot/grub
	cp bin/kernel.bin bin/isodir/boot/kernel.bin
	cp src/boot/grub.cfg bin/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ bin/isodir

run: bin/exomos.iso
	$(QEMU) -cdrom bin/exomos.iso

clean:
	rm -rf bin

.PHONY: all run clean