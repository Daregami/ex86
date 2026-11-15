
#include "drivers/vga.h"
#include "cpu/idt.h"

void kmain() {
    vga_init();
    idt_init();

    char *s = "Hello, world!";
    for (int i = 0; i < 151; i++) {
        kprintf(s);
    }
    kprintf(s);
    kprintf(s);
    kperror(s);

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}