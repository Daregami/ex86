#include "drivers/vga.h"
#include "cpu/ports.h"

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

void kperror(char *s) {
    uint16_t curr_attribute = attribute;
    vga_set_color(foreColour,backColourErr);
    kprintf(s);
    attribute = curr_attribute;
}

void kprintf(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        vga_putchar(s[i]);
        i++;
    }
    vga_cursor_move();
}