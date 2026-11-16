#include "kernel/syscall.h"
#include "cpu/idt.h"
#include "drivers/vga.h"

void syscalls_handler(reg_t regs) {
    kprintf("SYS!!!\n");
}
