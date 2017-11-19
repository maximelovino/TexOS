#include "multiboot.h"
#include "min_std_lib.h"
#include "gdt.h"
#include "display.h"
#include "idt.h"
#include "pic.h"

#ifdef TEST
void demo_mode();
void automated_tests();
void sleep_for_cancel();
void sleep_for_demo();
#endif

void kernel_entry(multiboot_info_t* multibootInfos) {
	gdt_init();
	display_init();
	pic_init();
	idt_init();
	sti();
	display_printf("GDT Initalized\nDisplay initialized\n");
	display_printf(
			"Hello and welcome to TexOS\nThe available memory is %d KB\n",
			multibootInfos->mem_upper);

#ifdef TEST
	demo_mode();
#endif

	while (1) {

	}
}
#ifdef TEST
void demo_mode() {
	sleep_for_demo();
	display_init();
	automated_tests();
	sleep_for_demo();
	//int x = 2 / 0;
	display_init();
	char* text = "Two things are infinite: the universe and human stupidity; and I'm not sure about the universe.";
	char smiley_face = 1;
	display_printf(
			"Here is a quote:\n%s\nAnd a smiley: %c and some random hexa numbers: %x and %x and their decimal value %d and %d\n",
			text, smiley_face, 0xB00B, 0xCAFE, 0xB00B, 0xCAFE);
	display_printf("Here we have a sentence with a tab\tright before this\n");
	display_printf("Displaying a negative number %d and %d\n", -129, 0);
	sleep_for_demo();
	display_init();
	for (int m = 0; m < DISPLAY_HEIGHT * DISPLAY_WIDTH; ++m) {
		print_char('A');
	}
	sleep_for_demo();
	print_char('B');
	sleep_for_demo();
	display_init();
	set_fg_color(COLOR_GREEN);
	display_printf("MATRIX TIME");
	sleep_for_demo();
	display_init();
	for (int k = 0; k < 100; ++k) {
		for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; ++i) {
			display_printf("%d", (i + (i / DISPLAY_WIDTH) + k) % 10);
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
	char* christmasMessage = "WE WISH YOU A MERRY CHRISTMAS";
	set_cursor_position(new_position);
	shift_cursor(-1 * (strlen(christmasMessage) / 2), 0);
	display_printf("%s",christmasMessage);
	set_bg_color(COLOR_DARK_GRAY);
	set_cursor_to_origin();
	for (int l = 0; l < DISPLAY_HEIGHT * DISPLAY_WIDTH; ++l) {
		sleep_for_cancel();
		print_char(0);
	}
	set_cursor_to_origin();
	set_fg_color(COLOR_BLUE);
	display_printf("If you watched until here, you should reconsider how you spend your time");
}

void automated_tests(){
	int test_int_itoa = 12345;
	char* test_string_itoa = "12345";
	char buffer_itoa[100];
	itoa(test_int_itoa,false,buffer_itoa);
	display_printf("ITOA TEST...%s\n", strncmp(buffer_itoa,test_string_itoa,5) == 0 ? "OK" : "FAIL");

	char* test_string_length = "1234567890";
	display_printf("STRLEN TEST...%s\n", strlen(test_string_length) == 10 ? "OK" : "FAIL");

	uint8_t test_fg_color = COLOR_GREEN;
	set_fg_color(test_fg_color);
	display_printf("SET FG COLOR TEST...");
	cursor_position_t cursor_position = get_cursor_position();
	cursor_position.x -= 1;
	char* pointer = get_vram_pointer(cursor_position);
	//We increment because first byte of a position is the char, second is the color
	pointer++;
	display_printf("%s\n", (*pointer & 0x0F) == test_fg_color ? "OK" : "FAIL");

	uint8_t test_bg_color = COLOR_WHITE;
	set_bg_color(test_bg_color);
	display_printf("SET BG COLOR TEST...");
	cursor_position = get_cursor_position();
	cursor_position.x -= 1;
	pointer = get_vram_pointer(cursor_position);
	//We increment because first byte of a position is the char, second is the color
	pointer++;
	display_printf("%s\n", ((*pointer >> 4) & 0x0F) == test_bg_color ? "OK" : "FAIL");

	set_bg_color(COLOR_BLACK);
	set_fg_color(COLOR_WHITE);
}

void sleep_for_cancel() {
	for (int i = 0; i < (5 * 10e4); ++i) {

	}
}

void sleep_for_demo() {
	for (int i = 0; i < (2 * 10e7); ++i) {

	}
}
#endif
