#ifndef PIC_H
#define PIC_H

#define MASTER_PORT 0x20
#define SLAVE_PORT  0xA0
#define EOI         0x20

void pic_remap();

#endif