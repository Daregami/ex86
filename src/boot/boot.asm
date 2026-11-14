bits 32

MALIGN      equ 1 << 0
MEMINFO     equ 1 << 1

GRUB_MAGIC  equ 0x1BADB002
GRUB_FLUGS  equ MEMINFO
GRUB_HASH   equ - (GRUB_MAGIC + GRUB_FLUGS)

global _start
extern kmain

section .multiboot
align 4
    dd GRUB_MAGIC
    dd GRUB_FLUGS
    dd GRUB_HASH

section .text
_start:
    cli
    mov esp, stack_up
    call kmain

.halt:
    cli
    hlt
    jmp .halt

section .bss
align 16
stack_down:
resb 8192
stack_up: