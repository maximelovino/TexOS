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
	enable_cursor();
}

void display_clear(){
	cursor_position_t start;
	start.x = 0;
	start.y = 0;
	display_clear_zone(start, DISPLAY_SIZE_BYTES);
}

void shift_cursor(int x_shift, int y_shift){
	currentPosition.x += x_shift;
	currentPosition.y += y_shift;
	draw_cursor();
}

void increment_cursor(){
	//TODO
}

void display_clear_zone(cursor_position_t start_coordinate, int count){
	//TODO do function to get memory address of coordinate on VRAM
	int start_position = (int)VGA_MEMORY + convert_2d_position(start_coordinate) * 2;

	for (int i = 0; i < count; i++) {
		if (i % 2 == 0) {
			//even bytes are null chars
			memset((void*)(start_position + i), 0, 1);
		}else{
			//odd bytes are color
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
	//TODO do for char '\n' and '\t'
	cursor_position_t cursor = getCursorPosition();
	if (convert_2d_position(cursor) >= (DISPLAY_HEIGHT * DISPLAY_WIDTH)) {
		scroll_screen();
		shift_cursor(0, -1);
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
}

cursor_position_t getCursorPosition(){
	return currentPosition;
}

void itoa(int value, int base, char* buffer){
	//TODO do it with base HEX as well
	//TODO temporary hack => do it better later
	buffer[0] = 0; //This is the null terminator
	int cnt = 1;
	while(value){
		char rem = value % 10;
		char tempBuffer[100];
		memcpy(tempBuffer, buffer, cnt);
		memcpy(&buffer[1], tempBuffer, cnt);
		buffer[0] = ZERO_CHAR_VALUE + rem;
		value /= 10;
		cnt++;
	}
}

void display_print(char* format, ...){
	//TODO you have to find it in the stack
	char* currentChar = format;
	int nextParamShift = 1;
	while (*currentChar) {
		if (strncmp(currentChar, "%d", 2) == 0) {
			int* value = (void*)&format + nextParamShift * 4;
			char buffer[100] = {0};
			itoa(*value, 10, buffer);
			printString(buffer);
			nextParamShift++;
			currentChar++;
		}else if(strncmp(currentChar, "%s", 2) == 0){
			currentChar++;
		}else if (strncmp(currentChar, "%x", 2) == 0) {
			currentChar++;
		}else if (strncmp(currentChar, "%c", 2) == 0){
			currentChar++;
		}else{
			printChar(*currentChar);
		}
		currentChar++;
	}
}

void enable_cursor(){
	//TODO this should take into account the current register value and only change bit 5
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
