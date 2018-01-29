#include "tex_lib.h"

void main() {
    puts("coucou depuis main\n");
    putc('a');
	putc('\n');
	stat_t stat;
	int fd = file_open("test");
	file_stat("test", &stat);
	char content[stat.size + 1];
	content[stat.size] = 0;
	file_read(fd, content, stat.size);
	file_close(fd);
	printf("%d -> %d\n", fd, stat.size);
	puts(content);
	char c = getc();
	putc(c);
	exec("shell");
	file_iterator_t it = file_iterator();
	char buffer[1024];
	while (file_has_next(&it)) {
		file_next(buffer, &it);
		printf("%s\n", buffer);
	}

	printf("ticks %d", get_ticks());

	while (getc() != 'q');
	printf("Exiting app...\n");
	exit();
}