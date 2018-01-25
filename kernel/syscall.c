// To avoid warnings if an argument is not used.
// Example:
// void f(int arg) {
//     UNUSED(arg);
// }

#include "../common/syscall_nb.h"
#include "../common/tex_fs.h"
#include "../common/types.h"
#include "display.h"
#include "keyboard.h"
#include "fs.h"
#include "timer.h"

#define UNUSED(x) ((void)(x))

static int syscall_puts(char* string);

static int syscall_exec(char* filename);

static int syscall_keypressed();

static int syscall_getc();

static int syscall_file_stat(char* filename, stat_t* stat);

static int syscall_file_open(char* filename);

static int syscall_file_close(int fd);

static int syscall_file_read(int fd, char* buf, uint count);

static int syscall_file_seek(int fd, uint offset);

static int syscall_file_iterator(file_iterator_t* iterator);

static int syscall_file_next(char* filename, file_iterator_t* iterator);

static int syscall_get_ticks();

static int syscall_sleep(uint ms);


// System call handler: call the appropriate system call according to the nb argument.
// Called by the assembly code _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4,
					uint32_t caller_tss_selector) {

	UNUSED(arg4);
	//TODO shift virtual memory pointer from task to "physical" memory pointer using the caller TSS
	switch (nb) {
		case SYSCALL_PUTS:
			return syscall_puts((char*) arg1);
		case SYSCALL_EXEC:
			return syscall_exec((char*) arg1);
		case SYSCALL_KEYPRESSED:
			return syscall_keypressed();
		case SYSCALL_GETC:
			return syscall_getc();
		case SYSCALL_FILE_STAT:
			return syscall_file_stat((char*) arg1, (stat_t*) arg2);
		case SYSCALL_FILE_OPEN:
			return syscall_file_open((char*) arg1);
		case SYSCALL_FILE_CLOSE:
			return syscall_file_close(arg1);
		case SYSCALL_FILE_READ:
			return syscall_file_read(arg1, (char*) arg3, arg2);
		case SYSCALL_FILE_SEEK:
			return syscall_file_seek(arg1, arg2);
		case SYSCALL_FILE_ITERATOR:
			return syscall_file_iterator((file_iterator_t*) arg1);
		case SYSCALL_FILE_NEXT:
			return syscall_file_next((char*) arg1, (file_iterator_t*) arg2);
		case SYSCALL_GET_TICKS:
			return syscall_get_ticks();
		case SYSCALL_SLEEP:
			return syscall_sleep(arg1);
	}
}

int syscall_puts(char* string) {
	display_printf("%s", string);
	return 0;
}

int syscall_exec(char* filename) {
	//TODO implement this
	return 0;
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
	//TODO should file next return something in case of problem?
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