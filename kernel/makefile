kernel.elf:bootstrap_asm.o kernel.o minStdLib.o gdt.o gdt_asm.o
	gcc $^ -Tkernel.ld -m32 -fno-builtin -ffreestanding -nostdlib -o $@

kernel.o:kernel.c kernel.h
	gcc -c -m32 $< -o $@

gdt_asm.o:gdt_asm.s
	nasm -f elf32 $< -o $@

gdt.o:gdt.c gdt.h
	gcc -c -m32 $< -o $@

minStdLib.o: minStdLib.c minStdLib.h
	gcc -c -m32 $< -o $@

bootstrap_asm.o:bootstrap_asm.s
	nasm -f elf32 $< -o $@

clean:
	rm *.o
	rm kernel.elf
