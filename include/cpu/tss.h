#ifndef TSS_H
#define TSS_H

#include <stdint.h>

typedef struct {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t unused[22];
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

void tss_init(int num, uint32_t kss, uint32_t kesp);
void tss_set_stack(uint32_t kss, uint32_t kesp);
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags_limit);

#endif