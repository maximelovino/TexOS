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
#include "../common/const.h"

#define SPLASH_SCREEN "taptouch.ss"


/**
 * Shows the game splash screen
 */
static void show_splash_screen();

/**
 * The game function
 */
static void game();

/**
 * Displays the score
 * @param score The score to display
 */
static void show_score(uint score);

void main() {
	srand(get_ticks());
	show_splash_screen();
	game();
	puts("\nBye bye see you soon.\n");
}

void game() {
	uint lives = 10;
	uint score = 0;
	char screen[DISPLAY_HEIGHT * DISPLAY_WIDTH + 1];
	screen[DISPLAY_SIZE] = '\0';
	memset(screen, ' ', DISPLAY_SIZE);
	while (1) {
		//set new letters to last row
		for (int column = 0; column < DISPLAY_WIDTH; ++column) {
			if (rand() % 5 == 0) {
				screen[DISPLAY_SIZE_WITHOUT_LAST_LINE + column] = 'a' + rand() % 26;
			}
		}
		puts(screen);
		sleep(1000);
		for (int column = 0; column < DISPLAY_WIDTH; ++column) {
			if (screen[column] != ' ') {
				lives--;
				if (lives <= 0) {
					show_score(score);
					return;
				}
				break;
			}
		}
		//shift up all lines
		memcpy(screen, screen + DISPLAY_WIDTH, DISPLAY_SIZE_WITHOUT_LAST_LINE);
		memset(screen + DISPLAY_SIZE_WITHOUT_LAST_LINE, ' ', DISPLAY_WIDTH);
		while (keypressed()) {
			char c = getc();
			if (c == ESCAPE_CHAR)
				return;
			for (int index = 0; index < DISPLAY_SIZE_WITHOUT_LAST_LINE; ++index) {
				if (c == screen[index]) {
					screen[index] = ' ';
					score++;
				}
			}
		}
	}
}

void show_score(uint score) {
	for (int i = 0; i <= DISPLAY_HEIGHT; ++i) {
		if (i == DISPLAY_HEIGHT / 2) {
			printf("                                    Score: %d\n", score);
		} else {
			putc('\n');
		}
	}
}

void show_splash_screen() {
	int fd = file_open(SPLASH_SCREEN);
	if (fd == -1) {
		printf("cannot find taptouch screen\n");
		exit();
	}
	stat_t stat;
	file_stat(SPLASH_SCREEN, &stat);
	char buf[stat.size + 1];
	buf[stat.size] = '\0';
	file_read(fd, buf, stat.size);
	file_close(fd);
	puts(buf);
	char pressed;
	while ((pressed = getc()) != ' ' && pressed != ESCAPE_CHAR);
	if (pressed == ESCAPE_CHAR) {
		puts("\nMaybe next time!\n");
		exit();
	}
}