#include "kernel.h"


void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	display_init();

	char* text = "HELLO WORLD THIS IS ME";
	char smileyFace = 1;
	display_printf("AVAILABLE MEMORY: %d KB\n", multibootInfos->mem_upper);
	display_printf("Here is a quote: %s and a smiley: %c and some random hexa numbers: %x and %x\n", text, smileyFace,
				   0xB00B, 0xCAFE);
	display_printf("Here we have a sentence with a tab\tright before this\n");
	for (int l = 0; l < 10e6; ++l) {

	}
	disable_cursor();
	display_printf("now cursor should be disabled\n");
	for (int i = 0; i < 10e7; ++i) {

	}
	enable_cursor();
	display_printf("and now enabled again\n");
	/*set_fg_color(COLOR_LIGHT_CYAN);
	display_print("THIS TEXT SHOULD BLUE ON BLACK\n");
	set_bg_color(COLOR_GREEN);
	set_fg_color(COLOR_RED);
	display_printf("THIS TEXT SHOULD RED ON GREEN\n");*/
	/*for (uint8_t i = 0; i < 16; ++i) {
		for (int k = 0; k < 10e7; ++k) {

		}
		set_fg_color(i);
		for (uint8_t j = 0; j < 16; ++j) {
			set_bg_color(j);
			display_printf("Hello %d %d\n",i,j);
		}
	}*/
	while (1) {

	}
}
