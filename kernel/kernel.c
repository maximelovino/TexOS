#include "kernel.h"


void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	display_init();


	char* text = "HELLO WORLD THIS IS ME";
	char smileyFace = 1;

	display_print("Here is a quote: %s and a smiley: %c and some random hexa numbers: %x and %x\n", text, smileyFace, 0xB00B, 0xCAFE);
	display_print("Here we have a sentence with a tab\tright before this\n");
	while (1) {

	}
}
