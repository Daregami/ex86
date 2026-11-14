#include "drivers/vga.h"

#define VGA_ADDR        0xB8000
#define VGA_WIDTH       80
#define VGA_HEIGHT      25

#define backColour      0
#define backColourErr   4
#define foreColour      15

static volatile uint16_t *buff = (volatile uint16_t*)(VGA_ADDR);
static uint16_t attribute;

static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

void vga_init() {
    vga_set_color(foreColour,backColour);
    vga_clear();
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    uint8_t attributeByte = (bg << 4) | (fg & 0x0F);
    attribute = attributeByte << 8;
}

void vga_set_cursor (uint16_t x, uint16_t y) {
    return;
}

void vga_clear() {
    for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++) {
        buff[i] = attribute + ' ';
    }
}

void vga_putchar(char c) { 
    buff[cursor_y * VGA_WIDTH + cursor_x] = attribute + c;
}

void kperror(char *s) {
    uint16_t curr_attribute = attribute;
    vga_set_color(foreColour,backColourErr);
    kprintf(s);
    attribute = curr_attribute;
}

void kprintf(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        switch (s[i]) {
            case '\n':
                cursor_x = 0;
                cursor_y++;
                break;
            default:
                vga_putchar(s[i]);
                cursor_x++;
                break;
        }
        i++;
    }
}