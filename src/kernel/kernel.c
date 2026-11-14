
void kmain() {
    volatile char *buff = (volatile char *)0xB8000;

    buff[0] = 0x06;

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}