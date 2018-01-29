# TexOS

TexOS is a kernel written as part of an Advanced Systems Programming class. TexOS runs on GRUB and implements a shell and the ability to compile user applications to run them on the kernel. It runs on the [IA-32 architecture](https://en.wikipedia.org/wiki/IA-32). You can find examples of the user applications in the [user folder](https://github.com/maximelovino/TexOS/tree/master/user). 

TexOS is also TexFS, a filesystem inspired by the popular Ext2 Filesystem, you can find tools to generate filesystem images in the [tools folder](https://github.com/maximelovino/TexOS/tree/master/tools).

## How to run the kernel?

You can run the kernel simply by doing `make` from the root of the project. The kernel will compile and will load an image with all our user applications. You need to have `gcc` and `qemu-system-i386` installed.