#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "../common/types.h"
#include "../common/const.h"

#ifndef CURSOR_LOOK_START
#define CURSOR_LOOK_START 0b0
#endif

#ifndef CURSOR_LOOK_END
#define CURSOR_LOOK_END 0b11111
#endif

#ifndef CURSOR_END_REGISTER
#define CURSOR_END_REGISTER 0xB
#endif

#ifndef CURSOR_START_REGISTER
#define CURSOR_START_REGISTER 0xA
#endif


#ifndef CURSOR_CMD_ADDRESS
#define CURSOR_CMD_ADDRESS 0x3d4
#endif

#ifndef CURSOR_DATA_ADDRESS
#define CURSOR_DATA_ADDRESS 0x3d5
#endif

extern void outb(uint16_t port, uint8_t data);

extern uint8_t inb(uint16_t port);

typedef struct cursor_position_st {
	uint8_t x;
	uint8_t y;
} cursor_position_t;

void shift_cursor(int x_shift, int y_shift);

void increment_cursor();

void enable_cursor();

void disable_cursor();

void set_cursor_default_look();

ushort convert_2d_to_1d_position(cursor_position_t cursor);

cursor_position_t convert_1d_to_2d_position(ushort position_1d);

void set_cursor_position(cursor_position_t position);

cursor_position_t get_cursor_position();

void write_cursor_to_memory();

void carriage_return();

#endif //_CURSOR_H_
