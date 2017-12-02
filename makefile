BUILD_FOLDER=build/
BUILD_KERNEL_FOLDER=$(BUILD_FOLDER)/kernel/
KERNEL_ELF_FILE=$(BUILD_KERNEL_FOLDER)kernel.elf
KERNEL_ISO=$(BUILD_KERNEL_FOLDER)kernel.iso
OS_FOLDER=$(BUILD_KERNEL_FOLDER)texos/
GRUB_OG_FOLDER=grub/
STAGE2=boot/grub/stage2_eltorito
TEST_FLAGS=

$(shell mkdir -p $(BUILD_FOLDER))

run:elf_file $(KERNEL_ISO)
	qemu-system-i386 -cdrom $(KERNEL_ISO)

test:TEST_FLAGS+=-DTEST
test:run

$(KERNEL_ISO):$(KERNEL_ELF_FILE)
	mkdir -p $(OS_FOLDER)boot
	cp $(KERNEL_ELF_FILE) $(OS_FOLDER)boot/kernel.elf
	cp -r $(GRUB_OG_FOLDER) $(OS_FOLDER)boot/
	genisoimage -R -b $(STAGE2) -input-charset utf8 -no-emul-boot -boot-info-table -o $(KERNEL_ISO) $(OS_FOLDER)

$(KERNEL_ELF_FILE): elf_file
elf_file:
	$(MAKE) -C kernel/ kernel TEST_FLAGS="$(TEST_FLAGS)"

build_tools:
	$(MAKE) -C tools/ all

.PHONY=run test elf_file build_tools

clean:
	@-rm -rf $(BUILD_FOLDER)

help:
	@echo "Usage:"
	@echo "\trun: Builds all the dependencies and runs the kernel in production mode"
	@echo "\ttest: Builds all the dependencies and runs the kernel in test mode"
	@echo "\t$(KERNEL_ISO): Builds all the dependencies and makes the iso file"
	@echo "\telf_file: Builds all the dependencies and makes the elf file"
	@echo "\tbuild_tools: Builds all tools"
	@echo "\tclean: cleans everything. You have to clean between test and run launches"
