kernel.elf:bootstrap_asm.o kernel.o minStdLib.o gdt.o gdt_asm.o pio_asm.o
	gcc -static -Tkernel.ld -m32 -ffreestanding -nostdlib $^ -o $@ -lgcc

kernel.o:kernel.c kernel.h
	gcc -c -m32 $< -o $@ -nostdlib -ffreestanding -Wall -Wextra

gdt_asm.o:gdt_asm.s
	nasm -f elf32 $< -o $@

gdt.o:gdt.c gdt.h
	gcc -c -m32 $< -o $@ -nostdlib -ffreestanding

minStdLib.o: minStdLib.c minStdLib.h
	gcc -c -m32 $< -o $@ -nostdlib -ffreestanding

bootstrap_asm.o:bootstrap_asm.s
	nasm -f elf32 $< -o $@

pio_asm.o:pio_asm.s
	nasm -f elf32 $< -o $@

clean:
	rm *.o
	rm kernel.elf
