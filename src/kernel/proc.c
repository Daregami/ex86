#include "kernel/proc.h"
#include "cpu/tss.h"

extern void usermode(uint32_t entry_point, uint32_t user_stack);

void enter_usermode() {
    uint8_t *user_code_area = (uint8_t *)0x40000000;

    user_code_area[0] = 0xCD; // int
    user_code_area[1] = 0x80; // 0x80 (вызов сискола)
    user_code_area[2] = 0xEB; // jmp short -2
    user_code_area[3] = 0xFE; // бесконечный цикл (while(1))

    uint32_t kesp;
    asm volatile("mov %%esp, %0" : "=r"(kesp));
    tss_set_stack(0x10, kesp);

    usermode((uint32_t)user_code_area, 0x40002000);
}