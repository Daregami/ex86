#include "kernel/proc.h"
#include "cpu/tss.h"

extern void usermode(uint32_t entry_point, uint32_t user_stack);

void user_test_func() {
    while (1);
}

void enter_usermode() {
    uint32_t kesp;
    asm volatile("mov %%esp, %0" : "=r"(kesp));
    tss_set_stack(0x10, kesp);

    usermode((uint32_t)&user_test_func, 0);
}