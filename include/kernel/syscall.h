#ifndef SYSCALL_H
#define SYSCALL_H

#include "cpu/idt.h"

void syscalls_handler(reg_t regs);

#endif