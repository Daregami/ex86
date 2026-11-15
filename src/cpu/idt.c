#include "cpu/idt.h"
#include "cpu/pic.h"
#include "cpu/ports.h"
#include "drivers/vga.h"

idt_entry_t idt_table[256] = {0};
idt_ptr_t ip;

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_table[num].base_lo = (base & 0xFFFF);
    idt_table[num].sel = sel;
    idt_table[num].always0 = 0;
    idt_table[num].flags = flags;
    idt_table[num].base_hi = (base >> 16) & 0xFFFF;
}

extern void irq0();
extern void idt_flush(uint32_t ip);

void idt_init() {
    pic_remap();

    idt_set_gate(32,(uint32_t)irq0,0x08,0x8E);

    ip.limit = (sizeof(idt_entry_t) * 256) - 1;
    ip.base = (uint32_t)&idt_table;

    idt_flush((uint32_t)&ip);
}

void irq_handler(reg_t regs) {
    if (regs.int_no >= 40) {
        outb(SLAVE_PORT, EOI);
    }
    outb(MASTER_PORT, EOI);

    kprintf("GOOD!\n");
}