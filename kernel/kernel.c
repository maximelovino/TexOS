#include "kernel.h"
#include "minStdLib.h"


void kernelEntry(multiboot_info_t* multibootInfos){
	void* vgaMem = (void*) 0xB8000;
	memset(vgaMem,0x00,4000); //This cleans the screen
	memset(vgaMem,0x0f,20); //This puts 10 white *

	while (1) {

	}
}
