#pragma once

#include <stdint.h>

#define MULTIBOOT_MAGIC     0x2BADB002
#define MULTIBOOT_FLAG_MMAP (1 << 6)
#define KERNEL_VIRT_BASE    0xC0000000

typedef struct {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type; // 1 = MEM FREE
} __attribute__((packed)) multiboot_mmap_entry_t;

typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed)) multiboot_info_t;

void parse_memory(uint32_t magic, multiboot_info_t *boot_phys);