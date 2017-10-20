#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include "../common/types.h"
#include "minStdLib.h"

//TODO constants for color
//TODO functions to set color BG and FG separately

#ifndef ZERO_CHAR_VALUE
#define ZERO_CHAR_VALUE 48
#endif

#ifndef VGA_MEMORY
#define VGA_MEMORY (void*) 0xB8000
#endif

#ifndef CURSOR_CMD_ADDRESS
#define CURSOR_CMD_ADDRESS 0x3d4
#endif

#ifndef CURSOR_DATA_ADDRESS
#define CURSOR_DATA_ADDRESS 0x3d5
#endif

#ifndef DISPLAY_SIZE_BYTES
#define DISPLAY_SIZE_BYTES 4000
#endif

#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 80
#endif

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 25
#endif

typedef struct cursor_position_st {
	uint8_t x;
	uint8_t y;
} cursor_position_t;

extern void outb(uint16_t port, uint8_t data);
extern void outw(uint16_t port, uint16_t data);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

//TODO enableCursor and disableCursor

void display_init();
void display_clear();
void set_colors(uint8_t colors);
uint8_t get_colors();
void printChar(char toPrint);
void printString(char* toPrint);
void setCursorPosition(cursor_position_t position);
cursor_position_t getCursorPosition();
void display_print(char* format, ...);
void draw_cursor();
void display_clear_zone(cursor_position_t start_coordinate, int count);
ushort convert_2d_position(cursor_position_t cursor);
cursor_position_t convert_1d_position(ushort position_1d);
void itoa(int value, int base, char* buffer);
void shift_cursor(int x_shift, int y_shift);
void increment_cursor();
void enable_cursor();

void scroll_screen();


#endif /* end of include guard: _DISPLAY_H_ */
