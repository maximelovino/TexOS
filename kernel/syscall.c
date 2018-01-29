/**
 * Syscall source file
 * @file 	syscall.c
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	January 29, 2018
 */

#include "../common/syscall_nb.h"
#include "../common/tex_fs.h"
#include "../common/types.h"
#include "display.h"
#include "keyboard.h"
#include "fs.h"
#include "timer.h"
#include "gdt.h"

#define UNUSED(x) ((void)(x))

/**
 * Puts syscall, displays a string on screen
 * @param string The string to display
 * @return 0
 */
static int syscall_puts(char* string);

/**
 * Exec syscall, executes a program
 * @param filename The name of the program
 * @return A status code
 */
static int syscall_exec(char* filename);

/**
 * Keypressed syscall, check if a key is pressed
 * @return 1 if key pressed, 0 otherwise
 */
static int syscall_keypressed();

/**
 * Get C syscall, blocking, gets the pressed character
 * @return The character pressed
 */
static int syscall_getc();

/**
 * File stat syscall, stat of a file
 * @param filename The filename to stat
 * @param stat The stat structure in which to write
 * @return 0 in case of success, -1 otherwise
 */
static int syscall_file_stat(char* filename, stat_t* stat);

/**
 * File open syscall, open a file
 * @param filename The filename to open
 * @return A file descriptor (>= 0) in case of success, -1 otherwise
 */
static int syscall_file_open(char* filename);

/**
 * File close syscall, closes a file
 * @param fd The file descriptor to close
 * @return 0
 */
static int syscall_file_close(int fd);

/**
 * File read syscall, reads a file
 * @param fd The file descriptor to read
 * @param buf The allocated buffer in which to read
 * @param count The number of bytes to read
 * @return The number of bytes read, 0 if the file was over, -1 in case of error
 */
static int syscall_file_read(int fd, char* buf, uint count);

/**
 * File seek syscall, seeks into a file
 * @param fd The file descriptor to seek
 * @param offset The offset to seek to from beginning of file
 * @return The offset seeked, or -1 in case of error
 */
static int syscall_file_seek(int fd, uint offset);

/**
 * File iterator syscall, gets a file iterator
 * @param iterator The iterator structure in which to put the iterator
 * @return 0
 */
static int syscall_file_iterator(file_iterator_t* iterator);

/**
 * File next syscall, gets the name of the next file
 * @param filename The buffer in which to put the name of the next file
 * @param iterator The file iterator
 * @return 0
 */
static int syscall_file_next(char* filename, file_iterator_t* iterator);

/**
 * Get ticks syscall, gets the ticks of the timer
 * @return The ticks of the timer
 */
static int syscall_get_ticks();

/**
 * Sleep syscall, sleeps for a given time
 * @param ms The number of milliseconds to sleep
 * @return 0
 */
static int syscall_sleep(uint ms);

/**
 * System call handler, calls the appropriate system call
 * @param nb The number of the syscall to launch
 * @param arg1 First arg of the syscall
 * @param arg2 Second arg of the syscall
 * @param arg3 Third arg of the syscall
 * @param arg4 Fourth arg of the syscall
 * @param caller_tss_selector The TSS selector of the calling task
 * @return The return value of the syscall
 */
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4,
					uint32_t caller_tss_selector) {
	void* address = (void*) (TASK_ADDR_SPACE * (SELECTOR_TO_GDT_INDEX(caller_tss_selector) - GDT_KERNEL_SIZE) / 2 +
							 TASK_START_ADDRESS);
	switch (nb) {
		case SYSCALL_PUTS:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_puts(address + arg1);
		case SYSCALL_EXEC:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_exec(address + arg1);
		case SYSCALL_KEYPRESSED:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_keypressed();
		case SYSCALL_GETC:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_getc();
		case SYSCALL_FILE_STAT:
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_stat(address + arg1, address + arg2);
		case SYSCALL_FILE_OPEN:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_open(address + arg1);
		case SYSCALL_FILE_CLOSE:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_close(arg1);
		case SYSCALL_FILE_READ:
			UNUSED(arg4);
			return syscall_file_read(arg1, address + arg2, arg3);
		case SYSCALL_FILE_SEEK:
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_seek(arg1, arg2);
		case SYSCALL_FILE_ITERATOR:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_iterator(address + arg1);
		case SYSCALL_FILE_NEXT:
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_next(address + arg1, address + arg2);
		case SYSCALL_GET_TICKS:
			UNUSED(arg1);
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_get_ticks();
		case SYSCALL_SLEEP:
			UNUSED(arg2);
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_sleep(arg1);
	}
	return -1;
}

int syscall_exec(char* filename) {
	return task_exec(filename);
}

int syscall_keypressed() {
	return keypressed();
}

int syscall_getc() {
	return getc();
}

int syscall_file_stat(char* filename, stat_t* stat) {
	return file_stat(filename, stat);
}

int syscall_file_open(char* filename) {
	return file_open(filename);
}

int syscall_file_close(int fd) {
	file_close(fd);
	return 0;
}

int syscall_file_read(int fd, char* buf, uint count) {
	return file_read(fd, buf, count);
}

int syscall_file_seek(int fd, uint offset) {
	return file_seek(fd, offset);
}

int syscall_file_iterator(file_iterator_t* iterator) {
	file_iterator_t it = file_iterator();
	memcpy(iterator, &it, sizeof(file_iterator_t));
	return 0;
}

int syscall_file_next(char* filename, file_iterator_t* iterator) {
	file_next(filename, iterator);
	return 0;
}

int syscall_get_ticks() {
	return get_ticks();
}

int syscall_sleep(uint ms) {
	sleep(ms);
	return 0;
}

int syscall_puts(char* string) {
	print_string(string);
	return 0;
}