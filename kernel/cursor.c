#include "cursor.h"

static cursor_position_t current_position;

void set_cursor_default_look() {
	outb(CURSOR_CMD_ADDRESS, CURSOR_START_REGISTER);
	outb(CURSOR_DATA_ADDRESS, CURSOR_FAT_LOOK_START);

	outb(CURSOR_CMD_ADDRESS, CURSOR_END_REGISTER);
	outb(CURSOR_DATA_ADDRESS, CURSOR_FAT_LOOK_END);
}

void shift_cursor(int x_shift, int y_shift) {
	current_position.x += x_shift;
	current_position.y += y_shift;
	write_cursor_to_memory();
}

void increment_cursor() {
	set_cursor_position(convert_1d_to_2d_position((convert_2d_to_1d_position(get_cursor_position()) + 1)));
}

void set_cursor_to_origin() {
	current_position.x = 0;
	current_position.y = 0;
	write_cursor_to_memory();
}

void carriage_return() {
	//TODO perhaps use shift
	current_position.x = 0;
	current_position.y++;
	set_cursor_position(current_position);
}

void set_cursor_position(cursor_position_t position) {
	current_position = position;
	write_cursor_to_memory();
}

void write_cursor_to_memory() {
	ushort position_1d = convert_2d_to_1d_position(current_position);

	outb(CURSOR_CMD_ADDRESS, 0xF); //LSB
	outb(CURSOR_DATA_ADDRESS, (uint8_t) (position_1d & 0xFF));

	outb(CURSOR_CMD_ADDRESS, 0xE); //MSB
	outb(CURSOR_DATA_ADDRESS, (uint8_t) (position_1d >> 8) & 0xFF);
}

cursor_position_t get_cursor_position() {
	return current_position;
}

void enable_cursor() {
	outb(CURSOR_CMD_ADDRESS, CURSOR_START_REGISTER);
	outb(CURSOR_DATA_ADDRESS, inb(CURSOR_DATA_ADDRESS) & (0 << 5));
}

void disable_cursor() {
	outb(CURSOR_CMD_ADDRESS, CURSOR_START_REGISTER);
	outb(CURSOR_DATA_ADDRESS, inb(CURSOR_DATA_ADDRESS) | (1 << 5));
}

ushort convert_2d_to_1d_position(cursor_position_t cursor) {
	return cursor.y * DISPLAY_WIDTH + cursor.x;
}

cursor_position_t convert_1d_to_2d_position(ushort position_1d) {
	cursor_position_t position;
	position.x = position_1d % DISPLAY_WIDTH;
	position.y = position_1d / DISPLAY_WIDTH;
	return position;
}