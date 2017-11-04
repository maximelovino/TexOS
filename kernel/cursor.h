#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "../common/types.h"
#include "../common/const.h"

#ifndef CURSOR_FAT_LOOK_START
#define CURSOR_FAT_LOOK_START 0b0
#endif

#ifndef CURSOR_FAT_LOOK_END
#define CURSOR_FAT_LOOK_END 0b11111
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

//TODO specifies end of line
/**
 * A function to shift the cursor according to the two axis
 * @param x_shift The shift on the x axis (horizontal shift)
 * @param y_shift The shift on the y axis (vertical shift)
 */
void shift_cursor(int x_shift, int y_shift);

/**
 * Function to move the cursor by one char further
 */
void increment_cursor();

/**
 * Function to set the cursor to the origin of the screen
 */
void set_cursor_to_origin();

/**
 * Function to enable the display of the cursor
 */
void enable_cursor();

/**
 * Function to disable the display of the cursor
 */
void disable_cursor();

/**
 * A function to set the cursor to its default look, block cursor
 */
void set_cursor_default_look();

/**
 * Function to convert the 2D position of a cursor to 1D
 * @param cursor The structure containing the 2D position of the cursor
 * @return The 1D value of the position
 */
//TODO really ushort here?
ushort convert_2d_to_1d_position(cursor_position_t cursor);

/**
 * Function to convert 1D position to 2D
 * @param position_1d The 1D position of the cursor
 * @return A structure position containing the 2D position of the given 1D position
 */
//TODO really ushort here?
cursor_position_t convert_1d_to_2d_position(ushort position_1d);

/**
 * Function to set the cursor to a specific position
 * @param position The 2D position to set
 */
void set_cursor_position(cursor_position_t position);

/**
 * Function to get the current cursor position
 * @return The current 2D cursor position
 */
cursor_position_t get_cursor_position();

/**
 * Function to write the current cursor value to memory
 */
//TODO this could be static in C file
void write_cursor_to_memory();

#endif //_CURSOR_H_
