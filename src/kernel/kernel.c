
#include "drivers/vga.h"
#include "cpu/idt.h"
#include "cpu/gdt.h"
#include "kernel/proc.h"
#include <stdint.h>
#include "kernel/multiboot.h"


void kmain(uint32_t magic, multiboot_info_t *boot_phys) {
    vga_init();
    gdt_init();
    idt_init();

    parse_memory(magic, boot_phys);

    //char *s = "Hello, world!\n";
    kprintf("Hello - %d\n", 55);

    //asm volatile ("int $32");

    //kprintf(s);

    asm volatile ("int $0x80");

    //asm volatile ("int $32");

    //kprintf("Hello - 0x%x\n", pt[1]);

    enter_usermode();

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}