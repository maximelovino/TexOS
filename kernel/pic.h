#ifndef _PIC_H_
#define _PIC_H_

#include "../common/types.h"

//External functions defined in pio_asm.s
extern void outb(uint16_t port, uint8_t data);
extern uint8_t inb(uint16_t port);

extern void pic_init();
extern void pic_eoi(int irq);

#endif

