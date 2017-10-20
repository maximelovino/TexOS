#include "kernel.h"

void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	display_init();
	display_print("%d %d",100,200);
}
