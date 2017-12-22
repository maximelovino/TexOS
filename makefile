BUILD_FOLDER=build/
BUILD_KERNEL_FOLDER=$(BUILD_FOLDER)kernel/
BUILD_TOOLS_FOLDER=$(BUILD_FOLDER)tools/
KERNEL_ELF_FILE=$(BUILD_KERNEL_FOLDER)kernel.elf
KERNEL_ISO=$(BUILD_KERNEL_FOLDER)kernel.iso
OS_FOLDER=$(BUILD_KERNEL_FOLDER)texos/
GRUB_OG_FOLDER=grub/
STAGE2=boot/grub/stage2_eltorito
TEST_FLAGS=
FS_TOOLS=fs_add fs_create fs_del fs_list fs_info
TOOLS=$(addprefix $(BUILD_TOOLS_FOLDER),$(FS_TOOLS))
IMAGE_CREATE=$(BUILD_TOOLS_FOLDER)fs_create
IMAGE_LABEL=tex
BLOCK_SIZE=2048
BLOCK_COUNT=10000
INODE_COUNT=100
IMAGE_ADD=$(BUILD_TOOLS_FOLDER)fs_add
FS_IMAGE=$(BUILD_FOLDER)fs.img

$(shell mkdir -p $(BUILD_FOLDER))

run:elf_file $(KERNEL_ISO) tools_build $(FS_IMAGE)
	qemu-system-i386 -cdrom $(KERNEL_ISO) -hda $(FS_IMAGE)

debug:elf_file $(KERNEL_ISO) tools_build $(FS_IMAGE)
	qemu-system-i386 -s -S -cdrom $(KERNEL_ISO) -hda $(FS_IMAGE)

test:TEST_FLAGS+=-DTEST
test:run

$(FS_IMAGE):$(TOOLS) common/splash common/splash2
	$(IMAGE_CREATE) $(IMAGE_LABEL) $(BLOCK_SIZE) $(BLOCK_COUNT) $(INODE_COUNT) $(FS_IMAGE)
	$(IMAGE_ADD) common/splash $(FS_IMAGE)
	$(IMAGE_ADD) common/splash2 $(FS_IMAGE)

$(KERNEL_ISO):$(KERNEL_ELF_FILE)
	mkdir -p $(OS_FOLDER)boot
	cp $(KERNEL_ELF_FILE) $(OS_FOLDER)boot/kernel.elf
	cp -r $(GRUB_OG_FOLDER) $(OS_FOLDER)boot/
	genisoimage -R -b $(STAGE2) -input-charset utf8 -no-emul-boot -boot-info-table -o $(KERNEL_ISO) $(OS_FOLDER)

$(KERNEL_ELF_FILE): elf_file
elf_file:
	$(MAKE) -C kernel/ kernel TEST_FLAGS="$(TEST_FLAGS)"

tools_build:
	$(MAKE) -C tools/ all

.PHONY=run test elf_file tools_build debug

clean:
	@-rm -rf $(BUILD_FOLDER)

help:
	@echo "Usage:"
	@echo "\trun: Builds all the dependencies and runs the kernel in production mode"
	@echo "\ttest: Builds all the dependencies and runs the kernel in test mode"
	@echo "\t$(KERNEL_ISO): Builds all the dependencies and makes the iso file"
	@echo "\telf_file: Builds all the dependencies and makes the elf file"
	@echo "\ttools_build: Builds all tools"
	@echo "\tclean: cleans everything. You have to clean between test and run launches"
