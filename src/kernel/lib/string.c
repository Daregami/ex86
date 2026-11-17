#include "kernel/lib/string.h"
#include <stdint.h>

void *memset(void *dest, int ch, size_t count) {
    uint8_t *d = (uint8_t *)dest;
    for (size_t i = 0; i < count; i++) {
        d[i] = (uint8_t)ch;
    }
    return dest;
}