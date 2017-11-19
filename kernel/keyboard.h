#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "pio_asm.h"
#include "display.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATE_PORT 0x64

#define ENTER_MAKE_CODE 0x1C
#define ENTER_BREAK_CODE 0x9C

#define LEFT_SHIFT_MAKE_CODE 0x2A
#define LEFT_SHIFT_BREAK_CODE 0xAA

#define RIGHT_SHIFT_MAKE_CODE 0x36
#define RIGHT_SHIFT_BREAK_CODE 0xB6

#define LEFT_CNTRL_MAKE_CODE 0x1D
#define LEFT_CNTRL_BREAK_CODE 0x9D

#define ALT_MAKE_CODE 0x38
#define ALT_BREAK_CODE 0xB8

#define KEYBOARD_BUFFER_SIZE 10


extern void keyboard_init();
extern void keyboard_handler();
extern int getc();
extern int keypressed();

#endif
