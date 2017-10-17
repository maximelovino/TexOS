#ifndef _KERNEL_H_
#define _KERNEL_H_
#include "multiboot.h"
#include "minStdLib.h"
#include "gdt.h"

void kernelEntry(multiboot_info_t* multibootInfos);

#endif /* end of include guard: _KERNEL_H_ */
