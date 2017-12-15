/**
 * IDT source file
 * @file 	idt.c
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	November 21, 2017
 */

#include "idt.h"


static idt_entry_t idt_table[256];

/**
 * Array of error messages for processor interrupts
 */
static char* processor_interruption_messages[21] = {
		"[0] Divide Error",
		"[1] Intel RESERVED exception number",
		"[2] External non maskable interrupt",
		"[3] Breakpoint",
		"[4] Overflow",
		"[5] Bound range exceeded",
		"[6] Invalid opcode",
		"[7] Device not available",
		"[8] Double fault",
		"[9] Coprocessor segment overrun",
		"[10] Invalid TSS",
		"[11] Segment not present",
		"[12] Stack-segment fault",
		"[13] General protection",
		"[14] Page fault",
		"[15] Intel RESERVED exception number",
		"[16] x87 FPU Floating-point error",
		"[17] Alignment check",
		"[18] Machine check",
		"[19] SIMD floating point exception",
		"[20] Virtualization exception"
};

/**
 * Builds and returns an IDT entry
 * @param selector The code segment selector to access the ISR
 * @param offset The adress of the ISR
 * @param type The IDT entry type
 * @param dpl The privilege level required to call the ISR
 * @return An IDT entry
 */
static idt_entry_t idt_build_entry(uint16_t selector, uint32_t offset, uint8_t type, uint8_t dpl) {
	idt_entry_t entry;
	entry.offset15_0 = offset & 0xffff;
	entry.selector = selector;
	entry.reserved = 0;
	entry.type = type;
	entry.dpl = dpl;
	entry.p = 1;
	entry.offset31_16 = (offset >> 16) & 0xffff;
	return entry;
}

void exception_handler(regs_t* regs) {
	set_bg_color(COLOR_BLACK);
	set_fg_color(COLOR_RED);
	display_printf("ERROR %s", processor_interruption_messages[regs->number]);
	halt();
}

void irq_handler(regs_t* regs) {
	if (regs->number == KEYBOARD_IRQ) {
		keyboard_handler();
	} else if (regs->number == TIMER_IRQ) {
		timer_handler();
	}
	pic_eoi(regs->number);
}

void idt_init() {
	idt_ptr_t idt_ptr;

	idt_ptr.base = (uint32_t) &idt_table;
	idt_ptr.limit = sizeof(idt_table) - 1;

	//Processor interrupts
	idt_table[0] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_0,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[1] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_1,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[2] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_2,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[3] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_3,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[4] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_4,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[5] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_5,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[6] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_6,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[7] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_7,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[8] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_8,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[9] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
								   (uint32_t) &proc_interrupt_9,
								   TYPE_INTERRUPT_GATE,
								   DPL_KERNEL);
	idt_table[10] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_10,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[11] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_11,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[12] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_12,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[13] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_13,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[14] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_14,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[15] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_15,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[16] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_16,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[17] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_17,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[18] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_18,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[19] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_19,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[20] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &proc_interrupt_20,
									TYPE_INTERRUPT_GATE,
									DPL_KERNEL);

	for (int i = 21; i < 32; ++i) {
		memset(&idt_table[i], 0, sizeof(idt_entry_t));
	}

	//IRQ
	idt_table[32] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_0,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[33] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_1,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[34] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_2,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[35] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_3,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[36] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_4,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[37] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_5,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[38] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_6,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[39] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_7,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[40] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_8,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[41] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t) &irq_9,
									TYPE_INTERRUPT_GATE, DPL_KERNEL);
	idt_table[42] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &irq_10, TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[43] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &irq_11, TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[44] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &irq_12, TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[45] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &irq_13, TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[46] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &irq_14, TYPE_INTERRUPT_GATE,
									DPL_KERNEL);
	idt_table[47] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,
									(uint32_t) &irq_15, TYPE_INTERRUPT_GATE,
									DPL_KERNEL);

	for (int i = 48; i < 256; ++i) {
		memset(&idt_table[i], 0, sizeof(idt_entry_t));
	}

	// Load the GDT
	idt_load(&idt_ptr);

}
