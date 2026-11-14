#include "drivers/vga.h"

#define VGA_ADDR        0xB8000
#define VGA_WIDTH       80
#define VGA_HEIGHT      25

#define backColour      0
#define backColourErr   4
#define foreColour      15

static uint16_t attribute = ((backColour << 4) | (foreColour & 0xFF)) << 8;
static uint16_t attributeErr = ((backColourErr << 4) | (foreColour & 0xFF)) << 8;

static volatile uint16_t *buff = (volatile uint16_t*)(VGA_ADDR);

static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;



void vga_putchar(char c) { 
    buff[cursor_y * VGA_WIDTH + cursor_x] = attribute + c;
}

void kperror(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        buff[cursor_y * VGA_WIDTH + cursor_x] = attributeErr + s[i];
        cursor_x++;
        i++;
    }
}

void kprintf(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        vga_putchar(s[i]);
        cursor_x++;
        i++;
    }
}