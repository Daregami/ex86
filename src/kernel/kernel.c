
#include "drivers/vga.h"
#include "cpu/idt.h"
#include "cpu/gdt.h"

void kmain() {
    vga_init();
    gdt_init();
    idt_init();

    char *s = "Hello, world!\n";
    kprintf(s);

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}