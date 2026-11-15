#include "cpu/idt.h"
#include "cpu/pic.h"

void idt_init() {
    pic_remap();
}