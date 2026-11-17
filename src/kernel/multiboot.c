#include "kernel/multiboot.h"
#include "drivers/vga.h"
#include <stdint.h>

void parse_memory(uint32_t magic, multiboot_info_t *boot_phys) {
    if (magic != MULTIBOOT_MAGIC) {
        kperror("Bad magic: %x\n", magic);
        __asm__ volatile ("cli; hlt");
    }

    multiboot_info_t *boot = (multiboot_info_t *)((uint32_t)boot_phys + KERNEL_VIRT_BASE);

    if (!(boot->flags & MULTIBOOT_FLAG_MMAP)) {
        kperror("No mmap!\n");
        __asm__ volatile ("cli; hlt");
    }

    uint32_t curr = boot->mmap_addr + KERNEL_VIRT_BASE;
    uint32_t end  = curr + boot->mmap_length;

    while (curr < end) {
        multiboot_mmap_entry_t *entry = (multiboot_mmap_entry_t *)curr;

        if (entry->addr < 0x100000000ULL && entry->type == 1) {
            uint32_t base = (uint32_t)entry->addr;
            uint32_t len = (uint32_t)entry->len;

            kprintf("FREE RAM: base=0x%x, len=0x%x\n", base, len);
        }

        curr += entry->size + sizeof(entry->size);
    }
}