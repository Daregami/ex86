
#include "drivers/vga.h"
#include "cpu/idt.h"
#include "cpu/gdt.h"

void kmain() {
    vga_init();
    gdt_init();
    idt_init();

    //char *s = "Hello, world!\n";
    kprintf("Hello - %d\n", 55);

    //asm volatile ("int $32");

    //kprintf(s);

    asm volatile ("int $0x80");

    //asm volatile ("int $32");

    kprintf("Hello - 0x%x\n", 55);

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}