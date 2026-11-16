#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags_limit;
    uint8_t base_high;
} __attribute__ ((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));

void gdt_init();
void tss_init(int num,uint32_t kss,uint32_t kesp);

#endif