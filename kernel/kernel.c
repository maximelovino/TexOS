#include "kernel.h"

void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	display_init();
	printString("HELLOWORLD");

	while (1) {

	}
}
