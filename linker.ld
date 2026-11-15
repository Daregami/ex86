ENTRY(_start)

KERNEL_VMA = 0xC0000000;
KERNEL_LMA = 0x00100000;

SECTIONS
{
    . = KERNEL_LMA;

    .boot.text :
    {
        *(.multiboot)
        *(.boot.text)
    }

    .boot.data :
    {
        *(.boot.data)
    }

    . += KERNEL_VMA;

    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA)
    {
        *(.text)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA)
    {
        *(.data)
        . = ALIGN(4096);
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA)
    {
        *(.rodata)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VMA)
    {
        *(.COMMON)
        *(.bss)
    }
}