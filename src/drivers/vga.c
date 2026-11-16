#include "drivers/vga.h"
#include "cpu/ports.h"
#include <stdarg.h>

#define VGA_ADDR        0xC00B8000
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

static void vga_cursor_move() {
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3D4,14);
    outb(0x3D5,cursorLocation >> 8);
    outb(0x3D4,15);
    outb(0x3D5,cursorLocation);
}

void vga_set_cursor(uint16_t x, uint16_t y) {
    cursor_x = x;
    cursor_y = y;
    vga_cursor_move();
}

void vga_clear() {
    for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++) {
        buff[i] = attribute + ' ';
    }
    vga_set_cursor(0,0);
}

static void vga_scroll() {
    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        buff[i] = buff[i + VGA_WIDTH];
    }

    uint16_t curr_attribute = attribute;
    vga_set_color(foreColour,backColour);

    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        buff[i] = attribute + ' ';
    }
    
    attribute = curr_attribute;

    cursor_y = VGA_HEIGHT - 1;
    cursor_x = 0;
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        buff[cursor_y * VGA_WIDTH + cursor_x] = attribute + c;
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        vga_scroll();
    }
}

void vga_puts(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        vga_putchar(str[i++]);
    }
}

void itoa(int32_t value, char* s, int base) {
    char digits[] = "0123456789ABCDEF";
    char buf[32];
    int i = 0;
    int j = 0;

    if (value < 0 && base == 10) {
        s[j++] = '-';
        value = -value;
    }

    do {
        int rem = value % base;
        buf[i++] = digits[rem];
        value /= base;
    } while(value > 0);

    while(i > 0) {
        s[j++] = buf[--i];
    }

    s[j] = '\0';
}

static void kvprintf(const char *s, va_list args) {
    char num[32];
    int i = 0;

    int32_t arg;

    while (s[i] != '\0') {
        if (s[i] == '%') {
            switch (s[i+1]) {
                case 'd':
                    arg = va_arg(args, int32_t);
                    itoa(arg, num, 10);
                    vga_puts(num);
                    i += 2;
                    break;
                default:
                    vga_putchar(s[i++]);
                    break;
            }
        } else {
            vga_putchar(s[i++]);
        }
    }
    vga_cursor_move();
    va_end(args);
}

void kprintf(const char *s, ...) {
    va_list args;
    va_start(args, s);
    kvprintf(s, args);
    va_end(args);
}

void kperror(const char *s, ...) {
    uint16_t curr_attribute = attribute;
    vga_set_color(foreColour, backColourErr);

    va_list args;
    va_start(args, s);
    kvprintf(s, args);
    va_end(args);

    attribute = curr_attribute;
}