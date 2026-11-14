
#include "drivers/vga.h"

void kmain() {
    vga_init();

    char *s = "Hello, world!";
    kprintf(s);
    kperror(s);
    kprintf(s);

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}