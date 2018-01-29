/**
 * Syscall numbers header file
 * @file 	syscall_nb.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	January 29, 2018
 */

#ifndef _SYSCALL_NB_H_
#define _SYSCALL_NB_H_

/**
 * Enum containing the available syscalls
 */
typedef enum {
	SYSCALL_PUTS,
	SYSCALL_EXEC,
	SYSCALL_KEYPRESSED,
	SYSCALL_GETC,
	SYSCALL_FILE_STAT,
	SYSCALL_FILE_OPEN,
	SYSCALL_FILE_CLOSE,
	SYSCALL_FILE_READ,
	SYSCALL_FILE_SEEK,
	SYSCALL_FILE_ITERATOR,
	SYSCALL_FILE_NEXT,
	SYSCALL_GET_TICKS,
	SYSCALL_SLEEP
} syscall_t;

#endif
