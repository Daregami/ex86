bits 32

MALIGN              equ 1 << 0
MEMINFO             equ 1 << 1

GRUB_MAGIC          equ 0x1BADB002
GRUB_FLUGS          equ MALIGN | MEMINFO
GRUB_HASH           equ - (GRUB_MAGIC + GRUB_FLUGS)

KERNEL_VIRT_ADDR    equ 0xC0000000
KERNEL_VIRT_INDEX   equ (KERNEL_VIRT_ADDR >> 22)

USER_ADDR           equ 0x40000000
USER_INDEX          equ (USER_ADDR >> 22)

global _start
extern kmain

section .multiboot
align 4
    dd GRUB_MAGIC
    dd GRUB_FLUGS
    dd GRUB_HASH

; global kernel_page_table

section .boot.data
align 4096
kernel_page_directory:
    ; 10000011 - present, r/w, PSE - 4mb
    dd 0x00000000 + 0x83
    times (USER_INDEX - 1) dd 0
    dd user_page_table + 0x07
    times (KERNEL_VIRT_INDEX - USER_INDEX - 1) dd 0
    ; times (KERNEL_VIRT_INDEX - 1) dd 0
    
    %assign i 0
    %rep 256
        dd (i << 22) | 0x83
        %assign i i+1
    %endrep

align 4096
user_page_table:
    %assign i 0
    %rep 1024
        dd (i * 0x1000 + 0x00800000) + 0x07
        %assign i i+1
    %endrep

section .boot.text
_start:
    cli

    mov edx, kernel_page_directory
    mov cr3, edx

    mov edx, cr4
    or edx, 0x00000010 ; PSE
    mov cr4, edx

    mov edx, cr0
    or edx, 0x80000000 ; 31 bit PG
    mov cr0, edx

    lea ecx, [high_addr]
    jmp ecx

section .text
high_addr:
    mov dword [kernel_page_directory], 0 
    mov ecx, cr3
    mov cr3, ecx

    mov esp, stack_up
    push ebx
    push eax
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