#include "cpu/gdt.h"

#define GDT_ENTRIES 6

extern void gdt_flush(uint32_t tgp);
extern void tss_flush();

static struct gdt_entry gdt_table[GDT_ENTRIES] = {0};
static struct gdt_ptr gp;

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags_limit;
    uint8_t base_high;
} __attribute__ ((packed));


void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags_limit) {
    gdt_table[num].limit_low = (limit & 0xFFFF);
    gdt_table[num].base_low = (base & 0xFFFF);
    gdt_table[num].base_middle = (base >> 16) & 0xFF;
    gdt_table[num].access = access;
    gdt_table[num].flags_limit = flags_limit;
    gdt_table[num].base_high = (base >> 24) & 0xFF;
}

void gdt_init() {
    gdt_set_gate(0,0,0,0,0);
    gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF);
    gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF);
    gdt_set_gate(3,0,0xFFFFFFFF,0xFA,0xCF);
    gdt_set_gate(4,0,0xFFFFFFFF,0xF2,0xCF);

    tss_init(5,0x10,0);

    gp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gp.base = (uint32_t)&gdt_table;

    gdt_flush((uint32_t)&gp);
    tss_flush();
}