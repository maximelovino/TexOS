// To avoid warnings if an argument is not used.
// Example:
// void f(int arg) {
//     UNUSED(arg);
// }

#include "../common/syscall_nb.h"
#include "../common/tex_fs.h"
#include "../common/types.h"

#define UNUSED(x) ((void)(x))

static int syscall_puts(char* string);

static int syscall_exec(char* filename);

static int syscall_keypressed();

static int syscall_getc();

static int syscall_file_stat(char* filename, stat_t* stat);

static int syscall_file_open(char* filename);

static int syscall_file_close();

static int syscall_file_read();

static int syscall_file_seek();

static int syscall_file_iterator();

static int syscall_file_next();

static int sycall_get_ticks();


// System call handler: call the appropriate system call according to the nb argument.
// Called by the assembly code _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4,
					uint32_t caller_tss_selector) {

}

