#include "kernel.h"

void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	display_init();

	for (int i = 0; i < 1999; i++) {
		printString("ABCDEFGHIJK");
	}

	for (int i = 0; i < 1e8; i++) {
		/* code */
	}

	printString("B");
	for (int i = 0; i < 1e8; i++) {
		/* code */
	}
	printString("C");
	for (int i = 0; i < 1e8; i++) {
		/* code */
	}
	printString(" Hello World");

	while (1) {

	}
}
