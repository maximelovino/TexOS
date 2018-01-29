/**
 * Shell source file
 *
 * @brief	This is the shell of TexOS
 * @file 	shell.c
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	January 29, 2018
 */

#include "tex_lib.h"

#define CMD_BUFFER_SIZE 512

char* quotes[] = {"\"Vous ecrivez une ligne de python vous avez deja 800k\" \n--F. Gluck",
				  "\"Dans Windows 98 une application pouvait encore faire torcher la machine\" \n--F. Gluck",
				  "\"Tu peux pas avoir 90% d'un jeu... Tu peux avoir 90% d'un systeme d'exploitation, Windows par exemple\" \n--Y. Rekik",
				  "\"Rappelez vous, c'est pas ca la programmation\" \n--M. Hoerdt en parlant de python",
				  "\"Python c'est fait pour planter\" \n--M. Hoerdt",
				  "En pointant du doigt la console python: \"Ca...ca c'est de la MERDE\" \n--M. Hoerdt",
				  "\"Touchez plus votre code, ca marche !\" \n--M. Hoerdt",
				  "\"Est-ce qu'un zombie peut sortir de son sleep()?\" \n--M. Hoerdt",
				  "\"Ouais... c'est du baratin marketing\" \n--F. Gluck parlant du projet de Rekik pour Raed"};

/**
 * This is the cat command available in the shell
 * @param filename The file to display
 */
static void cat(char* filename) {
	int fd = file_open(filename);
	if (fd == -1) {
		printf("cat: failed reading \"%s\"\n", filename);
		return;
	}
	int sz = 256;
	char buf[sz + 1];
	int n;
	do {
		n = file_read(fd, buf, sz);
		buf[n] = 0;
		puts(buf);

	} while (n);
	file_close(fd);
}

/**
 * This is the ls command available in the shell
 */
static void list_files() {
	file_iterator_t it = file_iterator();

	while (file_has_next(&it)) {
		char name[256];
		file_next(name, &it);
		stat_t stat;
		file_stat(name, &stat);
		printf("%s (%d)\n", name, stat.size);
	}
}

/**
 * This is the help command available in the shell
 */
static void help() {
	putc('\n');
	puts("ls        : list files present in the file system\n");
	puts("cat FILE  : dump the content of FILE to the screen\n");
	puts("run PROG  : execute the program PROG.\n");
	puts("sleep N   : sleep the specified number of milliseconds\n");
	puts("exit      : exit the shell\n");
}

/**
 * Function to read the entered command in the shell
 * @param buf The buffer in which to write the command
 */
static void read_command(char* buf) {
	char c = 0;
	int counter = 0;
	while ((c = getc()) != '\n') {
		if (c == '\b') {
			if (counter > 0) {
				buf--;
				counter--;
				putc(c);
			}
		} else if (c != '\t') {
			putc(c);
			*buf++ = c;
			counter++;
		}
	}
	*buf = 0;
}

static void run() {
	srand(get_ticks());
	printf("%s\n", quotes[rand() % 9]);

	char buf[CMD_BUFFER_SIZE];

	while (1) {
		puts("$ ");
		read_command(buf);
		char* line = trim(buf);  // remove heading and trailing spaces
		if (line[0] == 0) {
			putc('\n');
			continue;
		} else if (strcmp("help", line) == 0) {
			help();
		} else if (starts_with("run ", line)) {
			char* file = trim(line + strlen("run "));
			putc('\n');
			if (exec(file) == -1) {
				printf("Failed executing \"%s\"\n", file);
			}
		} else if (starts_with("cat ", line)) {
			putc('\n');
			cat(trim(line + strlen("cat ")));
		} else if (starts_with("sleep ", line)) {
			printf("debug'%s'", line + strlen("sleep "));
			uint ms = atoi(trim(line + strlen("sleep ")));
			putc('\n');
			printf("Sleeping for %dms...\n", ms);
			sleep(ms);
		} else if (strcmp("ls", line) == 0) {
			putc('\n');
			list_files();
		} else if (strcmp("exit", line) == 0) {
			puts("\nBye.\n");
			exit();
		} else if (strcmp("rand", line) == 0) {
			printf("\n%d\n", rand());
		} else {
			putc('\n');
			printf("%s: command not found\n", line);
		}
	}
}

void main() {
	run();
}
