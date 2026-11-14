
void kmain() {
    //void *buff = (void*)0xB8000;

    while(1) {
        __asm__ volatile ("cli; hlt");
    }
}