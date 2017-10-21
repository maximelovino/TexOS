CC=gcc
COMPILE_FLAGS=-m32 -nostdlib -ffreestanding -Wall -Wextra
LINKER_FLAGS=-Tkernel.ld -static -lgcc
COMPILE=$(CC) -c $(COMPILE_FLAGS)
LINK=$(CC) $(COMPILE_FLAGS) $(LINKER_FLAGS)
NASM=nasm -f elf32
ASM_OBJECTS=bootstrap_asm.o gdt_asm.o pio_asm.o
C_OBJECTS=kernel.o minStdLib.o gdt.o display.o cursor.o
BUILD_FOLDER=../build/
ASM_OBJECTS_FILES=$(addprefix $(BUILD_FOLDER),$(ASM_OBJECTS))
C_OBJECTS_FILES=$(addprefix $(BUILD_FOLDER),$(C_OBJECTS))
KERNEL_ELF_FILE=$(BUILD_FOLDER)kernel.elf
GRUB_OG_FOLDER=../grub
KERNEL_ISO=$(BUILD_FOLDER)kernel.iso
OS_FOLDER=$(BUILD_FOLDER)texos/
STAGE2=boot/grub/stage2_eltorito

$(shell mkdir -p $(BUILD_FOLDER))

.PHONY=run

run:$(KERNEL_ISO)
	qemu-system-i386 -cdrom $(KERNEL_ISO)

kernel:$(KERNEL_ISO)

$(KERNEL_ISO):$(KERNEL_ELF_FILE)
	mkdir -p $(OS_FOLDER)boot
	cp $(KERNEL_ELF_FILE) $(OS_FOLDER)boot/kernel.elf
	cp -r $(GRUB_OG_FOLDER) $(OS_FOLDER)boot/
	genisoimage -R -b $(STAGE2) -input-charset utf8 -no-emul-boot -boot-info-table -o $(KERNEL_ISO) $(OS_FOLDER)

$(KERNEL_ELF_FILE):$(ASM_OBJECTS_FILES) $(C_OBJECTS_FILES)
	$(LINK) $^ -o $@

$(BUILD_FOLDER)kernel.o:kernel.c kernel.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)gdt_asm.o:gdt_asm.s
	$(NASM) $< -o $@

$(BUILD_FOLDER)display.o: display.c display.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)cursor.o: cursor.c cursor.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)gdt.o:gdt.c gdt.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)minStdLib.o: minStdLib.c minStdLib.h
	$(COMPILE) $< -o $@

$(BUILD_FOLDER)bootstrap_asm.o:bootstrap_asm.s
	$(NASM) $< -o $@

$(BUILD_FOLDER)pio_asm.o:pio_asm.s
	$(NASM) $< -o $@

clean:
	rm -r $(BUILD_FOLDER)
