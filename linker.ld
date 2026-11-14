ENTRY(_start)

SECTIONS
{
    . = 1M;

    .text :
    {
        *(.multiboot)
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        . = ALIGN(4096);
    }

    .rodata :
    {
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss :
    {
        *(.bss)
        . = ALIGN(4096);
    }
}