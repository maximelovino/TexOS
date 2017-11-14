CC=gcc
TEST_FLAGS=
COMPILE_FLAGS=-m32 -nostdlib -ffreestanding -Wall -Wextra $(TEST_FLAGS)
LINKER_FLAGS=-Tkernel.ld -static -lgcc
COMPILE=$(CC) -c $(COMPILE_FLAGS)
LINK=$(CC) $(COMPILE_FLAGS) $(LINKER_FLAGS)
NASM=nasm -f elf32
ASM_OBJECTS=bootstrap_asm.o gdt_asm.o pio_asm.o idt_asm.o
C_OBJECTS=kernel.o min_std_lib.o gdt.o display.o cursor.o idt.o pic.o keyboard.o
BUILD_FOLDER=../build/
ASM_OBJECTS_FILES=$(addprefix $(BUILD_FOLDER),$(ASM_OBJECTS))
C_OBJECTS_FILES=$(addprefix $(BUILD_FOLDER),$(C_OBJECTS))
KERNEL_ELF_FILE=$(BUILD_FOLDER)kernel.elf
GRUB_OG_FOLDER=../grub
KERNEL_ISO=$(BUILD_FOLDER)kernel.iso
OS_FOLDER=$(BUILD_FOLDER)texos/
STAGE2=boot/grub/stage2_eltorito
VGA_COLORS_HEADER=../common/vga_colors.h
TYPES_HEADER=../common/types.h
COMMON_HEADERS=$(TYPES_HEADER) ../common/const.h

$(shell mkdir -p $(BUILD_FOLDER))

kernel:$(KERNEL_ELF_FILE)
	@echo "Kernel built"

$(KERNEL_ELF_FILE):$(ASM_OBJECTS_FILES) $(C_OBJECTS_FILES)
	$(LINK) $^ -o $@

$(BUILD_FOLDER)kernel.o:kernel.c multiboot.h min_std_lib.h gdt.h display.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)gdt_asm.o:gdt_asm.s const.inc
	$(NASM) $< -o $@

$(BUILD_FOLDER)idt.o:idt.c idt.h pic.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)keyboard.o:keyboard.c keyboard.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)pic.o:pic.c pic.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)idt_asm.o:idt_asm.s const.inc
	$(NASM) $< -o $@

$(BUILD_FOLDER)display.o:display.c display.h $(COMMON_HEADERS) x86.h min_std_lib.h $(VGA_COLORS_HEADER) cursor.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)cursor.o:cursor.c cursor.h pio_asm.h $(COMMON_HEADERS)
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)gdt.o:gdt.c gdt.h min_std_lib.h x86.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)min_std_lib.o:min_std_lib.c min_std_lib.h $(TYPES_HEADER)
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)bootstrap_asm.o:bootstrap_asm.s
	$(NASM) $< -o $@

$(BUILD_FOLDER)pio_asm.o:pio_asm.s
	$(NASM) $< -o $@

clean:
	@-rm -rf $(BUILD_FOLDER)
