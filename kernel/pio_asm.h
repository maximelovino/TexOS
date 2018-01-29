/**
 * PIO_ASM header file
 *
 * @brief	This file contains the declarations of the functions defined in pio_asm.s
 * @file 	pio_asm.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 21, 2017
 */

#ifndef PIO_ASM_H_
#define PIO_ASM_H_

#include "../common/types.h"

/**
 * Function to write an 8 bit value to a PIO peripheral
 * @param port The port to write to
 * @param data The data to write
 */
extern void outb(uint16_t port, uint8_t data);

/**
 * Function to read an 8 bit value from a PIO peripheral
 * @param port The port to read from
 * @return The data read from the port
 */
extern uint8_t inb(uint16_t port);

/**
 * Function to write a 16 bit value to a PIO peripheral
 * @param port The port to write to
 * @param data The data to write
 */
extern void outw(uint16_t port, uint16_t data);

/**
 * Function to read a 16 bit value from a PIO peripheral
 * @param port The port to read from
 * @return The data read from the port
 */
extern uint16_t inw(uint16_t port);

#endif
