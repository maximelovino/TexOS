// Setup GDT index 4 to store the task's TSS descriptor
// Setup GDT index 5 to store the task's LDT descriptor
void task_setup() {
	static tss_t task_tss;
	memset(&task_tss, 0, sizeof(tss_t));
	static gdt_entry_t task_ldt[2];

	// Add the task's TSS and LDT to the GDT
	gdt[4] = gdt_make_tss(&task_tss, DPL_KERNEL);
	gdt[5] = gdt_make_ldt((uint32_t) task_ldt, sizeof(task_ldt) - 1, DPL_KERNEL);
	int gdt_tss_sel = gdt_entry_to_selector(&gdt[4]);
	int gdt_ldt_sel = gdt_entry_to_selector(&gdt[5]);

	// Define code and data segments in the LDT; both segments are overlapping
	static uint8_t addr_space[32768] __attribute__((aligned(4096)));  // 32KB of address space
	int ldt_code_idx = 0;  // Index of code segment descriptor in the LDT
	int ldt_data_idx = 1;  // Index of data segment descriptor in the LDT
	uint limit = sizeof(addr_space);  // Limit for both code and data segments
	task_ldt[ldt_code_idx] = gdt_make_code_segment(addr_space, limit / 4096, DPL_USER);
	task_ldt[ldt_data_idx] = gdt_make_data_segment(addr_space, limit / 4096, DPL_USER);

	// Initialize the TSS fields
	// The LDT selector must point to the task's LDT
	task_tss.ldt_selector = gdt_ldt_sel;

	// Setup code and stack pointers
	task_tss.eip = 0;
	task_tss.esp = task_tss.ebp = limit;  // stack pointers

	// Code and data segment selectors are in the LDT
	task_tss.cs = GDT_INDEX_TO_SELECTOR(ldt_code_idx) | DPL_USER | LDT_SELECTOR;
	task_tss.ds = task_tss.es = task_tss.fs = task_tss.gs = task_tss.ss =
			GDT_INDEX_TO_SELECTOR(ldt_data_idx) | DPL_USER | LDT_SELECTOR;
	task_tss.eflags = 512;  // Activate hardware interrupts (bit 9)

	// Task's kernel stack
	static uint8_t task_kernel_stack[8192];
	task_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	task_tss.esp0 = (uint32_t)(task_kernel_stack) + sizeof(task_kernel_stack);
}

