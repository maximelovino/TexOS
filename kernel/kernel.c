/**
 * Kernel source file
 * @brief	Entry point of our OS
 * @file 	kernel.c
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	November 3, 2017
 */

#include "multiboot.h"
#include "min_std_lib.h"
#include "gdt.h"
#include "display.h"
#include "idt.h"
#include "test.h"
#include "pic.h"
#include "timer.h"


void kernel_entry(multiboot_info_t* multibootInfos) {
	gdt_init();
	display_init();
	idt_init();
	pic_init();
	timer_init(100);
	sti();
	display_printf("GDT Initalized\nDisplay initialized\n");
	display_printf(
			"Hello and welcome to TexOS\nThe available memory is %d KB\n",
			multibootInfos->mem_upper);

#ifdef TEST
	demo_mode();
#endif
	int toPrint;
	while (1) {
		toPrint = getc();
		if (toPrint == 'Q') {
			display_init();
			display_printf("System shutting down NOW");
			halt();
		} else if (toPrint == ESCAPE_CHAR) {
			display_init();
		} else if (toPrint != UNKNOWN_KEY) {
			display_printf("%c", toPrint);

		}
	}
}