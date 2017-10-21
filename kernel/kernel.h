#ifndef _KERNEL_H_
#define _KERNEL_H_
#include "multiboot.h"
#include "minStdLib.h"
#include "gdt.h"
#include "display.h"

void kernelEntry(multiboot_info_t* multibootInfos);

void sleep_for_cancel();

void sleep_for_demo();

#endif /* end of include guard: _KERNEL_H_ */
