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
	cursor_position_t start;
	start.x = 0;
	start.y = 0;
	display_clear_zone(start, DISPLAY_SIZE_BYTES);
}

void display_clear_zone(cursor_position_t start_coordinate, int count){
	//TODO do function to get memory address of coordinate on VRAM
	int start_position = (int)VGA_MEMORY + convert_2d_position(start_coordinate) * 2;

	for (int i = 0; i < count; i++) {
		if (i % 2 == 0) {
			//even bytes are null chars
			memset((void*)(start_position + i), 0, 1);
		}else{
			memset((void*)(start_position + i), color, 1);
		}
	}
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
	setCursorPosition(convert_1d_position(position_1d + 1));
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
	ushort position_1d = convert_2d_position(currentPosition);

	outb(CURSOR_CMD_ADDRESS, 0xF); //LSB
	outb(CURSOR_DATA_ADDRESS, (uint8_t)(position_1d & 0xFF));

	outb(CURSOR_CMD_ADDRESS, 0xE); //MSB
	outb(CURSOR_DATA_ADDRESS, (uint8_t)(position_1d >> 8) & 0xFF);
	display_cursor();
}

cursor_position_t getCursorPosition(){
	return currentPosition;
}

void display_print(char* format, ...){

}

void display_cursor(){
	outb(CURSOR_CMD_ADDRESS, 0xA);
	outb(CURSOR_DATA_ADDRESS,0xE);

	outb(CURSOR_CMD_ADDRESS, 0xB);
	outb(CURSOR_DATA_ADDRESS,0x1f);
}

ushort convert_2d_position(cursor_position_t cursor){
	return cursor.y * DISPLAY_WIDTH + cursor.x;
}

cursor_position_t convert_1d_position(ushort position_1d){
	cursor_position_t position;
	position.x = position_1d % DISPLAY_WIDTH;
	position.y = position_1d / DISPLAY_WIDTH;
	return position;
}

void scroll_screen(){
	memcpy(VGA_MEMORY, VGA_MEMORY  + (DISPLAY_WIDTH * 2), DISPLAY_SIZE_BYTES - (DISPLAY_WIDTH * 2));
	// We clear from start of last line for a whole line
	cursor_position_t start_clear;
	start_clear.x = 0;
	start_clear.y = DISPLAY_HEIGHT - 1;
	display_clear_zone(start_clear, DISPLAY_WIDTH * 2);
}
