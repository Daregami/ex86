#include "drivers/pit.h"
#include "cpu/idt.h"

void timer(reg_t regs) {
    (void)regs;
}

void pit_init() {
    register_interrupt_handler(32, timer);
}