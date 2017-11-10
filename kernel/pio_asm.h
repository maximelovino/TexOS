#ifndef PIO_ASM_H_
#define PIO_ASM_H_

#include "../common/types.h"

extern void outb(uint16_t port, uint8_t data);

extern uint8_t inb(uint16_t port);

#endif
