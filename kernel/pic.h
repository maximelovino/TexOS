#ifndef _PIC_H_
#define _PIC_H_

#include "pio_asm.h"

extern void pic_init();
extern void pic_eoi(int irq);

#endif
