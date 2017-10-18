#include "display.h"

static uint8_t color = 0x0F;
static cursor_position_t currentPosition;
//TODO Store cursor position here

void display_init(){
	cursor_position_t newPos;
	newPos.x = 0;
	newPos.y = 0;
	display_clear();
	setCursorPosition(newPos);
}

void display_clear(){
	memset(VGA_MEMORY,0,DISPLAY_SIZE_BYTES);
}

void set_colors(uint8_t colors){
	color = colors;
}

uint8_t get_colors(){
	return color;
}

void printChar(char toPrint){
	cursor_position_t cursor = getCursorPosition();
	if (convert_2d_position(cursor) >= (DISPLAY_HEIGHT * DISPLAY_WIDTH)) {
		scroll_screen();
		cursor.y--;
		setCursorPosition(cursor);
	}

	uint16_t position_1d = convert_2d_position(cursor);
	void* address = VGA_MEMORY + (position_1d * 2);
	memset(address, toPrint, 1); // first the character
	memset(address + 1, color, 1); //then the color
	setCursorPosition(convert_1d_position(convert_2d_position(cursor) + 1));
}

void printString(char* toPrint){
	int i = 0;
	while (toPrint[i]) {
		printChar(toPrint[i]);
		i++;
	}
}

void setCursorPosition(cursor_position_t position){
	currentPosition = position;
	draw_cursor();
}

void draw_cursor(){
	uint16_t position_1d = convert_2d_position(currentPosition);
	outb(CURSOR_CMD_ADDRESS, 0xE); //MSB first
	outb(CURSOR_DATA_ADDRESS, (position_1d >> 8));
	outb(CURSOR_CMD_ADDRESS, 0xF); //LSB last
	outb(CURSOR_DATA_ADDRESS, (position_1d & 0xFF));
}

cursor_position_t getCursorPosition(){
	return currentPosition;
}

void display_print(char* format, ...){

}

uint16_t convert_2d_position(cursor_position_t cursor){
	return cursor.y * DISPLAY_WIDTH + cursor.x;
}

cursor_position_t convert_1d_position(uint16_t position_1d){
	cursor_position_t position;
	position.x = position_1d % DISPLAY_WIDTH;
	position.y = position_1d / DISPLAY_WIDTH;
	return position;
}

void scroll_screen(){
	memcpy(VGA_MEMORY, VGA_MEMORY  + (DISPLAY_WIDTH * 2), DISPLAY_SIZE_BYTES - (DISPLAY_WIDTH * 2));
	memset(VGA_MEMORY + DISPLAY_WIDTH * (DISPLAY_HEIGHT-1) * 2, 0, DISPLAY_WIDTH * 2);
}
