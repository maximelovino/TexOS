/**
 * IDT header file
 * @file 	idt.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	November 21, 2017
 */

#ifndef _IDT_H_
#define _IDT_H_

#include "../common/types.h"
#include "x86.h"
#include "../common/min_std_lib.h"
#include "display.h"
#include "pic.h"
#include "keyboard.h"
#include "timer.h"
#include "syscall_asm.h"

#define KEYBOARD_IRQ 1
#define TIMER_IRQ 0

/**
 * IDT function for processor interrupt 0, defined in idt_asm.s
 */
extern void proc_interrupt_0();

/**
 * IDT function for processor interrupt 1, defined in idt_asm.s
 */
extern void proc_interrupt_1();

/**
 * IDT function for processor interrupt 2, defined in idt_asm.s
 */
extern void proc_interrupt_2();

/**
 * IDT function for processor interrupt 3, defined in idt_asm.s
 */
extern void proc_interrupt_3();

/**
 * IDT function for processor interrupt 4, defined in idt_asm.s
 */
extern void proc_interrupt_4();

/**
 * IDT function for processor interrupt 5, defined in idt_asm.s
 */
extern void proc_interrupt_5();

/**
 * IDT function for processor interrupt 6, defined in idt_asm.s
 */
extern void proc_interrupt_6();

/**
 * IDT function for processor interrupt 7, defined in idt_asm.s
 */
extern void proc_interrupt_7();

/**
 * IDT function for processor interrupt 8, defined in idt_asm.s
 */
extern void proc_interrupt_8();

/**
 * IDT function for processor interrupt 9, defined in idt_asm.s
 */
extern void proc_interrupt_9();

/**
 * IDT function for processor interrupt 10, defined in idt_asm.s
 */
extern void proc_interrupt_10();

/**
 * IDT function for processor interrupt 11, defined in idt_asm.s
 */
extern void proc_interrupt_11();

/**
 * IDT function for processor interrupt 12, defined in idt_asm.s
 */
extern void proc_interrupt_12();

/**
 * IDT function for processor interrupt 13, defined in idt_asm.s
 */
extern void proc_interrupt_13();

/**
 * IDT function for processor interrupt 14, defined in idt_asm.s
 */
extern void proc_interrupt_14();

/**
 * IDT function for processor interrupt 15, defined in idt_asm.s
 */
extern void proc_interrupt_15();

/**
 * IDT function for processor interrupt 16, defined in idt_asm.s
 */
extern void proc_interrupt_16();

/**
 * IDT function for processor interrupt 17, defined in idt_asm.s
 */
extern void proc_interrupt_17();

/**
 * IDT function for processor interrupt 18, defined in idt_asm.s
 */
extern void proc_interrupt_18();

/**
 * IDT function for processor interrupt 19, defined in idt_asm.s
 */
extern void proc_interrupt_19();

/**
 * IDT function for processor interrupt 20, defined in idt_asm.s
 */
extern void proc_interrupt_20();

/**
 * IDT function for IRQ 0, defined in idt_asm.s
 */
extern void irq_0();

/**
 * IDT function for IRQ 1, defined in idt_asm.s
 */
extern void irq_1();

/**
 * IDT function for IRQ 2, defined in idt_asm.s
 */
extern void irq_2();

/**
 * IDT function for IRQ 3, defined in idt_asm.s
 */
extern void irq_3();

/**
 * IDT function for IRQ 4, defined in idt_asm.s
 */
extern void irq_4();

/**
 * IDT function for IRQ 5, defined in idt_asm.s
 */
extern void irq_5();

/**
 * IDT function for IRQ 6, defined in idt_asm.s
 */
extern void irq_6();

/**
 * IDT function for IRQ 7, defined in idt_asm.s
 */
extern void irq_7();

/**
 * IDT function for IRQ 8, defined in idt_asm.s
 */
extern void irq_8();

/**
 * IDT function for IRQ 9, defined in idt_asm.s
 */
extern void irq_9();

/**
 * IDT function for IRQ 10, defined in idt_asm.s
 */
extern void irq_10();

/**
 * IDT function for IRQ 11, defined in idt_asm.s
 */
extern void irq_11();

/**
 * IDT function for IRQ 12, defined in idt_asm.s
 */
extern void irq_12();

/**
 * IDT function for IRQ 13, defined in idt_asm.s
 */
extern void irq_13();

/**
 * IDT function for IRQ 14, defined in idt_asm.s
 */
extern void irq_14();

/**
 * IDT function for IRQ 15, defined in idt_asm.s
 */
extern void irq_15();

/**
 * CPU context used when saving/restoring context from an interrupt
 */
typedef struct regs_st {
	uint32_t gs, fs, es, ds;
	uint32_t ebp, edi, esi;
	uint32_t edx, ecx, ebx, eax;
	uint32_t number, error_code;
	uint32_t eip, cs, eflags, esp, ss;
} regs_t;


/**
 * Structure of an IDT descriptor. There are 3 types of descriptors:
 * a task-gate, an interrupt-gate, and a trap-gate.
 * See 5.11 of Intel 64 & IA32 architectures software developer's manual for more details.
 * For task gates, offset must be 0.
 */
typedef struct idt_entry_st {
	uint16_t offset15_0;   // only used by trap and interrupt gates
	uint16_t selector;     // segment selector for trap and interrupt gates; TSS segment selector for task gates
	uint16_t reserved : 8;
	uint16_t type : 5;
	uint16_t dpl : 2;
	uint16_t p : 1;

	uint16_t offset31_16;  // only used by trap and interrupt gates
} __attribute__((packed)) idt_entry_t;


/**
 * Structure describing a pointer to the IDT gate table.
 * This format is required by the lgdt instruction.
 */
typedef struct idt_ptr_st {
	uint16_t limit;   // Limit of the table (ie. its size)
	uint32_t base;    // Address of the first entry
} __attribute__((packed)) idt_ptr_t;

/**
 * Handler function for processor exception
 * Displays an error message according to the type of processor exception
 * @param regs The CPU context when calling the function (contains the number of the exception)
 */
extern void exception_handler(regs_t* regs);

/**
 * Handler function for IRQ
 * Handles keyboard and timer IRQ by passing them along to their respective handlers
 * @param regs The CPU context when calling the function (contains the number of the IRQ)
 */
extern void irq_handler(regs_t* regs);

/**
 * Function to intialize the IDT
 */
extern void idt_init();

/**
 * Function to load the IDT in memory, defined in idt_asm.s
 * @param ptr A pointer structure for the IDT descriptor table
 */
extern void idt_load(idt_ptr_t* ptr);

#endif
