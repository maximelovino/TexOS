#include "kernel.h"

void kernelEntry(multiboot_info_t* multibootInfos) {
	gdt_init();
	display_init();

#ifdef TEST
	demo_mode();
#else
	display_printf("Hello and welcome to TexOS\nThe available memory is %d KB",multibootInfos->mem_upper);
#endif

	while (1) {

	}
}

void demo_mode() {
	char* text = "Two things are infinite: the universe and human stupidity; and I'm not sure about the universe.";
	char smiley_face = 1;
	display_printf(
			"Here is a quote:\n%s\nAnd a smiley: %c and some random hexa numbers: %x and %x and their decimal value %d and %d\n",
			text, smiley_face, 0xB00B, 0xCAFE, 0xB00B, 0xCAFE);
	display_printf("Here we have a sentence with a tab\tright before this\n");
	sleep_for_demo();
	display_init();
	set_fg_color(COLOR_GREEN);
	display_printf("MATRIX TIME");
	sleep_for_demo();
	display_init();
	for (int k = 0; k < 100; ++k) {
		for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; ++i) {
			display_printf("%d", (i + k) % 10);
		}
	}
	sleep_for_demo();
	display_init();
	set_bg_color(COLOR_RED);
	for (int j = 0; j < DISPLAY_WIDTH * DISPLAY_HEIGHT; ++j) {
		print_char(0);

	}
	cursor_position_t new_position;
	new_position.x = DISPLAY_WIDTH / 2;
	new_position.y = DISPLAY_HEIGHT / 2;
	set_cursor_position(new_position);
	display_printf("WE WISH YOU A MERRY CHRISTMAS");
	set_bg_color(COLOR_DARK_GRAY);
	set_cursor_to_origin();
	for (int l = 0; l < DISPLAY_HEIGHT * DISPLAY_WIDTH; ++l) {
		sleep_for_cancel();
		print_char(0);
	}
	set_cursor_to_origin();
	set_fg_color(COLOR_RED);
	display_printf("If you watched until here, you should reconsider how you spend your time");
}

void sleep_for_cancel() {
	for (int i = 0; i < (5 * 10e4); ++i) {
		
	}
}

void sleep_for_demo() {
	for (int i = 0; i < (2 * 10e7); ++i) {

	}
}
