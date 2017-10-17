#include "kernel.h"

void kernelEntry(multiboot_info_t* multibootInfos){
	gdt_init();
	void* vgaMem = (void*) 0xB8000;
	memset(vgaMem,0x00,4000); //This cleans the screen
	memset(vgaMem,0x0f,1); //This puts 10 white *
	memset(vgaMem + 1,0xf0,1);

	while (1) {

	}
}
