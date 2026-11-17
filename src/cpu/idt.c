#include "cpu/idt.h"
#include "cpu/pic.h"
#include "cpu/ports.h"
#include "drivers/vga.h"
#include "kernel/syscall.h"

idt_entry_t idt_table[256] = {0};
idt_ptr_t ip;

static isr_t interrupt_handlers[256] = {0};

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_table[num].base_lo = (base & 0xFFFF);
    idt_table[num].sel = sel;
    idt_table[num].always0 = 0;
    idt_table[num].flags = flags;
    idt_table[num].base_hi = (base >> 16) & 0xFFFF;
}

extern void irq0(); // timer
extern void isr128(); // syscalls
extern void idt_flush(uint32_t ip);
extern uint32_t isr_stub_table[32];

void idt_init() {
    pic_remap();

    for (int i = 0; i < 32; i++) {
        idt_set_gate(i,isr_stub_table[i],0x08,0x8E);
    }

    idt_set_gate(32,(uint32_t)irq0,0x08,0x8E);

    idt_set_gate(128,(uint32_t)isr128,0x08,0x8E + 0x60);
    register_interrupt_handler(128,syscalls_handler);

    ip.limit = (sizeof(idt_entry_t) * 256) - 1;
    ip.base = (uint32_t)&idt_table;

    idt_flush((uint32_t)&ip);
}

void interrupt_handler(reg_t regs) {
    if (regs.int_no >=32 && regs.int_no <= 47) {
        if (regs.int_no >= 40) {
            outb(SLAVE_PORT, EOI);
        }
        outb(MASTER_PORT, EOI);
    }

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else if (regs.int_no < 32) {
        kperror("CPU Exception! Vector: %d, Err: %d\n", regs.int_no, regs.err_code);
        while(1) {
            __asm__ volatile ("cli; hlt");
        }
    }
}