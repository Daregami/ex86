#include "cpu/tss.h"

tss_entry_t tss = {0};

void tss_init(int num, uint32_t kss, uint32_t kesp) {
    uint32_t base = (uint32_t)&tss;
    uint32_t limit = sizeof(tss_entry_t) - 1;

    gdt_set_gate(num, base, limit, 0x89, 0);

    tss.ss0 = kss;
    tss.esp0 = kesp;
    tss.iomap_base = sizeof(tss_entry_t);
}

void tss_set_stack(uint32_t kss, uint32_t kesp) {
    tss.ss0 = kss;
    tss.esp0 = kesp;
}

