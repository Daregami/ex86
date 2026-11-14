bits 32

global _start
extern kmain

section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

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