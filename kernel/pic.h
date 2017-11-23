/**
 * PIC header file
 * @file 	pic.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	November 21, 2017
 */

#ifndef _PIC_H_
#define _PIC_H_

#include "pio_asm.h"

/**
 * Initialize the PICs by remapping IRQs 0-15 to 32-47
 * More details here: http://wiki.osdev.org/8259_PIC
 */
extern void pic_init();

/**
 * Sends an end-of-interrupt to the PICs.
 * An EOI must also be sent to the slave for IRQs > 7
 * @param irq The irq to clear
 */
extern void pic_eoi(int irq);

#endif

