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
#include "gdt.h"

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
    print_string("handler mdr\n");
	void *address = (void*)(TASK_ADDR_SPACE * (SELECTOR_TO_GDT_INDEX(caller_tss_selector) - GDT_KERNEL_SIZE) / 2 + 0x800000);
	display_printf("%x\n", address + arg1);
	display_printf("%s\n", address + arg1);
	switch (nb){
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
			return syscall_file_close(*(int*)(address + arg1));
		case SYSCALL_FILE_READ:
			UNUSED(arg4);
			return syscall_file_read(*(int*)(address + arg1), address + arg2, *(uint*)(address + arg3));
		case SYSCALL_FILE_SEEK:
			UNUSED(arg3);
			UNUSED(arg4);
			return syscall_file_seek(*(int*)(address + arg1), *(uint*)(address + arg2));
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
			return syscall_sleep(*(uint*)(address + arg1));
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

int syscall_puts(char* string){
	print_string("coucou");
    print_string(string);
    return 0;
}