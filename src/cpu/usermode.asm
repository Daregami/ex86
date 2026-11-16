global usermode

usermode:
    mov ecx, [esp + 4]
    mov edx, [esp + 8]

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push 0x23 ; ss
    push edx ; esp

    pushfd ; eflags
    pop eax
    or eax, 0x200 ; IF flag
    push eax

    push 0x1B ; cs
    push ecx ; eip

    iret
