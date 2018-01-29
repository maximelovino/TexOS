/**
 * Syscall asm header file
 * @file 	syscall_asm.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	January 29, 2018
 */

#ifndef _SYSCALL_ASM_H_
#define _SYSCALL_ASM_H_

/**
 * The assembly syscall handler function called by the IRQ
 */
extern void _syscall_handler();

#endif
