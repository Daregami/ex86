
#include "drivers/vga.h"

void kmain() {
    char *s = "Hello, world!";
    kprintf(s);
    kperror(s);

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}