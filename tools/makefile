CC=gcc
COMPILE_FLAGS=-Wall -Wextra
COMPILE=$(CC) -c $(COMPILE_FLAGS)
LINK=$(CC) $(COMPILE_FLAGS)

BUILD_FOLDER=../build/tools/
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(BUILD_FOLDER)%.o,$(SRCS))
DEPS=$(OBJS:.o=.d)

$(shell mkdir -p $(BUILD_FOLDER))

all: $(BUILD_FOLDER)fs_create $(BUILD_FOLDER)fs_add $(BUILD_FOLDER)fs_info

$(BUILD_FOLDER)fs_add: $(BUILD_FOLDER)fs_add.o $(BUILD_FOLDER)tex_fs_tools.o
	$(LINK) $^ -o $@

$(BUILD_FOLDER)fs_info: $(BUILD_FOLDER)fs_info.o $(BUILD_FOLDER)tex_fs_tools.o
	$(LINK) $^ -o $@

$(BUILD_FOLDER)fs_create: $(BUILD_FOLDER)fs_create.o $(BUILD_FOLDER)tex_fs_tools.o
	$(LINK) $^ -o $@

$(BUILD_FOLDER)%.o:%.c
	$(COMPILE) -MMD $< -o $@

clean:
	@-rm -rf $(BUILD_FOLDER)

-include $(DEPS)