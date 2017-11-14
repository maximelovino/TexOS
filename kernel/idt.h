#ifndef _IDT_H_
#define _IDT_H_

#include "../common/types.h"
#include "x86.h"
#include "min_std_lib.h"
#include "display.h"
#include "pic.h"
#include "keyboard.h"

extern void proc_interrupt_0();

extern void proc_interrupt_1();

extern void proc_interrupt_2();

extern void proc_interrupt_3();

extern void proc_interrupt_4();

extern void proc_interrupt_5();

extern void proc_interrupt_6();

extern void proc_interrupt_7();

extern void proc_interrupt_8();

extern void proc_interrupt_9();

extern void proc_interrupt_10();

extern void proc_interrupt_11();

extern void proc_interrupt_12();

extern void proc_interrupt_13();

extern void proc_interrupt_14();

extern void proc_interrupt_15();

extern void proc_interrupt_16();

extern void proc_interrupt_17();

extern void proc_interrupt_18();

extern void proc_interrupt_19();

extern void proc_interrupt_20();

extern void irq_0();

extern void irq_1();

extern void irq_2();

extern void irq_3();

extern void irq_4();

extern void irq_5();

extern void irq_6();

extern void irq_7();

extern void irq_8();

extern void irq_9();

extern void irq_10();

extern void irq_11();

extern void irq_12();

extern void irq_13();

extern void irq_14();

extern void irq_15();

typedef struct regs_st {
	uint32_t gs, fs, es, ds;
	uint32_t ebp, edi, esi;
	uint32_t edx, ecx, ebx, eax;
	uint32_t number, error_code;
	uint32_t eip, cs, eflags, esp, ss;
} regs_t;

// Structure of an IDT descriptor. There are 3 types of descriptors:
// a task-gate, an interrupt-gate, and a trap-gate.
// See 5.11 of Intel 64 & IA32 architectures software developer's manual for more details.
// For task gates, offset must be 0.
typedef struct idt_entry_st {
	uint16_t offset15_0;   // only used by trap and interrupt gates
	uint16_t selector;     // segment selector for trap and interrupt gates; TSS segment selector for task gates
	uint16_t reserved : 8;
	uint16_t type : 5;
	uint16_t dpl : 2;
	uint16_t p : 1;

	uint16_t offset31_16;  // only used by trap and interrupt gates
} __attribute__((packed)) idt_entry_t;

// Structure describing a pointer to the IDT gate table.
// This format is required by the lgdt instruction.
typedef struct idt_ptr_st {
	uint16_t limit;   // Limit of the table (ie. its size)
	uint32_t base;    // Address of the first entry
} __attribute__((packed)) idt_ptr_t;

extern void exception_handler(regs_t* regs);

extern void irq_handler(regs_t* regs);

extern void idt_init();

extern void idt_load(idt_ptr_t* ptr);
#endif
