CC=gcc
TEST_FLAGS=
COMPILE_FLAGS=-m32 -g -nostdlib -ffreestanding -Wall -Wextra $(TEST_FLAGS)
LINKER_FLAGS=-Tkernel.ld -static -lgcc
COMPILE=$(CC) -c $(COMPILE_FLAGS)
LINK=$(CC) $(COMPILE_FLAGS) $(LINKER_FLAGS)
NASM=nasm -g -f elf32

BUILD_FOLDER=../build/kernel/
SRCS=$(wildcard *.c *.s)
OBJS=$(patsubst %.c,$(BUILD_FOLDER)%.o,$(patsubst %.s,$(BUILD_FOLDER)%.o,$(SRCS))) $(BUILD_FOLDER)tex_fs.o $(BUILD_FOLDER)min_std_lib.o
DEPS=$(OBJS:.o=.d)
KERNEL_ELF_FILE=$(BUILD_FOLDER)kernel.elf

$(shell mkdir -p $(BUILD_FOLDER))

kernel:$(KERNEL_ELF_FILE)
	@echo "Kernel built"

$(KERNEL_ELF_FILE):kernel.ld $(OBJS)
	$(LINK) $(OBJS) -o $@

$(BUILD_FOLDER)%.o:%.s
	$(NASM) $< -o $@

$(BUILD_FOLDER)%.o:%.c
	$(COMPILE) -MMD $< -o $@

$(BUILD_FOLDER)%.o:../common/%.c
	$(COMPILE) -MMD $< -o $@

clean:
	@-rm -rf $(BUILD_FOLDER)

-include $(DEPS)
