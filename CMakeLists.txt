cmake_minimum_required(VERSION 3.8)
project(TexOS)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        common/types.h
        grub/menu.lst
        grub/stage2_eltorito
        kernel/pic.c
        kernel/pic.h
        kernel/idt.c
        kernel/idt.h
        kernel/idt_asm.s
        kernel/keyboard.c
        kernel/keyboard.h
        kernel/timer.c
        kernel/timer.h
        kernel/bootstrap_asm.s
        kernel/const.inc
        kernel/display.c
        kernel/display.h
        kernel/gdt.c
        kernel/gdt.h
        kernel/gdt_asm.s
        kernel/kernel.c
        kernel/kernel.ld
        kernel/makefile
        kernel/min_std_lib.c
        kernel/min_std_lib.h
        kernel/multiboot.h
        kernel/pio_asm.s
        kernel/ide.h
        kernel/ide.c
        common/vga_colors.h
        kernel/x86.h
        tools/makefile
        common/syscall_nb.h
        user/app.c
        user/entrypoint_asm.s
        user/shell.c
        user/syscall_asm.s
        user/app.ld
        kernel/syscall.c
        kernel/syscall_asm.s
        kernel/task.c
        kernel/task.h
        kernel/task_asm.s
        makefile kernel/cursor.c kernel/cursor.h common/const.h kernel/pio_asm.h kernel/test.c kernel/test.h tools/fs_create.c tools/tex_fs_tools.c tools/tex_fs_tools.h common/tex_fs.h tools/fs_add.c tools/fs_info.c tools/fs_list.c tools/fs_del.c kernel/fs.c kernel/fs.h kernel/syscall_asm.h user/tex_lib.c user/tex_lib.h)


set(FS_CREATE_DEPS common/tex_fs.h tools/fs_create.c tools/tex_fs_tools.h tools/tex_fs_tools.c)
set(FS_ADD_DEPS common/tex_fs.h tools/fs_add.c tools/tex_fs_tools.h tools/tex_fs_tools.c common/tex_fs.c)


add_executable(fs_create ${FS_CREATE_DEPS})
add_executable(fs_add ${FS_ADD_DEPS})

add_executable(TexOS ${SOURCE_FILES})