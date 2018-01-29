CC=gcc
TEST_FLAGS=
COMPILE_FLAGS=-m32 -g -nostdlib -ffreestanding -Wall -Wextra $(TEST_FLAGS) -fno-pie
LINKER_FLAGS=-Tapp.ld
COMPILE=$(CC) -c $(COMPILE_FLAGS)
LINK=$(CC) $(COMPILE_FLAGS) $(LINKER_FLAGS)
NASM=nasm -g -f elf32

all: app shell

app: app.o entrypoint_asm.o tex_lib.o syscall_asm.o min_std_lib.o
	$(LINK) $^ -o $@
app.o: app.c
	$(COMPILE) $< -o $@
shell: shell.o tex_lib.o syscall_asm.o entrypoint_asm.o min_std_lib.o
	$(LINK) $^ -o $@
tex_lib.o: tex_lib.c tex_lib.h
	$(COMPILE) $< -o $@
shell.o: shell.c
	$(COMPILE) $< -o $@
syscall_asm.o: syscall_asm.s
	$(NASM) $^
entrypoint_asm.o: entrypoint_asm.s
	$(NASM) $^
min_std_lib.o: ../common/min_std_lib.c ../common/min_std_lib.h
	$(COMPILE) $< -o $@
clean:
	@-rm *.o shell app

-include $(DEPS)
