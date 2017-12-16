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
#include "ide.h"
#include "test.h"
#include "pic.h"
#include "timer.h"
#include "fs.h"


void kernel_entry(multiboot_info_t* multiboot_infos) {
	gdt_init();
	display_init();
	display_printf("Display initialized\nGDT initialized\n");
	idt_init();
	display_printf("IDT initialized\n");
	pic_init();
	display_printf("PIC initialized\n");
	timer_init(100);
	display_printf("Timer initialized\n");
	sti();
	display_printf("Enabled interruptions\n");
	display_printf(
			"Hello and welcome to TexOS\nThe available memory is %d KB\n",
			multiboot_infos->mem_upper);

	tex_fs_superblock_t sb;
	memset(&sb, 0, sizeof(tex_fs_superblock_t));
	read_superblock(&sb);

	tex_fs_metadata_t fs;
	fs.superblock = &sb;

	uint8_t block_map[fs.superblock->block_count];
	memset(block_map, 0, fs.superblock->block_count);
	fs.block_map = block_map;

	uint8_t inode_map[fs.superblock->inode_count];
	memset(inode_map, 0, fs.superblock->inode_count);
	fs.inode_map = inode_map;

	tex_fs_inode_t inode_list[fs.superblock->inode_count];
	memset(inode_list, 0, fs.superblock->inode_count * sizeof(tex_fs_inode_t));
	fs.inode_list = inode_list;
	read_image(&fs);


	display_printf("%d\n", fs.block_map[0]);

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