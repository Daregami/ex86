bits 32

MALIGN              equ 1 << 0
MEMINFO             equ 1 << 1

GRUB_MAGIC          equ 0x1BADB002
GRUB_FLUGS          equ MALIGN | MEMINFO
GRUB_HASH           equ - (GRUB_MAGIC + GRUB_FLUGS)

KERNEL_VIRT_ADDR    equ 0xC0000000
KERNEL_VIRT_INDEX   equ (KERNEL_VIRT_ADDR >> 22)

global _start
extern kmain

section .multiboot
align 4
    dd GRUB_MAGIC
    dd GRUB_FLUGS
    dd GRUB_HASH

section .boot.data
align 4096
boot_page_directory:
    ; 00000011 - present, r/w
    dd boot_page_table + 0x03
    times (KERNEL_VIRT_INDEX - 1) dd 0
    dd boot_page_table + 0x03
    times (1024 - KERNEL_VIRT_INDEX - 1) dd 0

align 4096
boot_page_table:
    %assign i 0
    %rep 1024
        dd (i * 0x1000) + 0x03
        %assign i i+1
    %endrep

section .boot.text
_start:
    cli

    mov eax, boot_page_directory
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000 ; 31 bit PG
    mov cr0, eax

    lea ecx, [high_addr]
    jmp ecx

section .text
high_addr:
    mov dword [boot_page_directory], 0 
    mov ecx, cr3
    mov cr3, ecx

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