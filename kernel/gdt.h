/**
 * GDT header file
 * @file 	gdt.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 3, 2017
 */

#ifndef _GDT_H_
#define _GDT_H_

#include "../common/min_std_lib.h"
#include "x86.h"

#define TASK_ADDR_SPACE 0x100000// 1MB of address space
#define GDT_KERNEL_SIZE 4 // null, code, data, tss
#define SELECTOR_TO_GDT_INDEX(idx) ((idx) >> 3)
#define GDT_INDEX_TO_SELECTOR(idx) ((idx) << 3)
#define MAX_TASKS 8
#define GDT_KERNEL_ENTRIES 4

#define TASK_START_ADDRESS 0x400000
#define TASK_STACK_SIZE 65536
/**
 * Structure of a GDT descriptor. There are 2 types of descriptors: segments and TSS.
 * Section 3.4.5 of Intel 64 & IA32 architectures software developer's manual describes
 * segment descriptors while section 6.2.2 describes TSS descriptors.
 */
typedef struct gdt_entry_st {
	uint16_t lim15_0;
	uint16_t base15_0;
	uint8_t base23_16;

	uint8_t type : 4;
	uint8_t s : 1;
	uint8_t dpl : 2;
	uint8_t present : 1;

	uint8_t lim19_16 : 4;
	uint8_t avl : 1;
	uint8_t l : 1;
	uint8_t db : 1;
	uint8_t granularity : 1;

	uint8_t base31_24;
} __attribute__((packed)) gdt_entry_t;

/**
 * Structure describing a pointer to the GDT descriptor table.
 * This format is required by the lgdt instruction.
 */
typedef struct gdt_ptr_st {
	uint16_t limit;    // Limit of the table (ie. its size)
	uint32_t base;     // Address of the first entry
} __attribute__((packed)) gdt_ptr_t;

/**
 * Function to initialize the GDT
 */
extern void gdt_init();

/**
 * Function to load the GDT in memory, written in assembly, definition in gdt_asm.s
 * @param gdt_ptr A pointer structure for the GDT descriptor table
 */
extern void gdt_load(gdt_ptr_t* gdt_ptr);

/**
 * Executes a program specified by the filename
 * @param filename The filename of the program
 * @return A status code
 */
extern int task_exec(char* filename);

#endif
