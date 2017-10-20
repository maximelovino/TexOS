#include "kernel.h"


void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	display_init();

	for (int j = 0; j < 30; j++) {
		display_print("Hello %d\n",j);
		for (int k = 0; k < 10e6; k++) {
		}
	}

	display_print("%x and %x",0xB00B, 0xCAFE);
	while (1) {

	}
}
