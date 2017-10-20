#include "display.h"

static uint8_t color = (COLOR_BLACK << 4) | COLOR_WHITE;
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
	write_cursor_to_memory();
}

void increment_cursor(){
	setCursorPosition(convert_1d_position((convert_2d_position(getCursorPosition()) + 1)));
}

void carriage_return(){
	currentPosition.x = 0;
	currentPosition.y++;
	setCursorPosition(currentPosition);
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
	//TODO BE CAREFUL WHEN SHIFTING CURSOR FOR '\n' because we could go too far outside the screen, so -1 would not be enough
	//TODO CHECK THIS, WHEN WE '\n' from last line it doesn't work
	//TODO SHIFT CURSOR STILL NOT WORKING
	cursor_position_t cursor = getCursorPosition();
	if (convert_2d_position(cursor) >= (DISPLAY_HEIGHT * DISPLAY_WIDTH)) {
		scroll_screen();
		shift_cursor(0, -1);
		cursor = getCursorPosition();
	}
	if (toPrint == '\n') {
		carriage_return();
		return;
	}
	if (toPrint == '\t') {
		for (char i = 0; i < TAB_SIZE; i++) {
			printChar(' ');
		}
		return;
	}

	uint16_t position_1d = convert_2d_position(cursor);
	void* address = VGA_MEMORY + (position_1d * 2);
	memset(address, toPrint, 1); // first the character
	memset(address + 1, color, 1); //then the color
	increment_cursor();
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
	write_cursor_to_memory();
}

void write_cursor_to_memory(){
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
	int current = 0;
	if (value == 0) {
		buffer[0] = ZERO_CHAR_VALUE;
		return;
	}
	while(value){
		char rem = value % base;
		char toAppend;
		if (rem >= 10) {
			toAppend = 'A' + rem - 10;
		}else{
			toAppend = ZERO_CHAR_VALUE + rem;
		}
		buffer[current] = toAppend;
		value /= base;
		current++;
	}
	char tempBuffer[100];
	for (int i = 0; i < current; i++) {
		tempBuffer[i] = buffer[current - i - 1];
	}
	memcpy(buffer, tempBuffer, current);
}

void display_print(char* format, ...){
	char* currentChar = format;
	int nextParamShift = 1;
	while (*currentChar) {
		if (strncmp(currentChar, "%d", 2) == 0) {
			int* value = (void*)&format + nextParamShift * STACK_JUMP;
			char buffer[100] = {0};
			itoa(*value, 10, buffer);
			printString(buffer);
			nextParamShift++;
			currentChar++;
		}else if(strncmp(currentChar, "%x", 2) == 0){
			int* hexValue = (void*)&format + nextParamShift * STACK_JUMP;
			char hexBuffer[100] = {0};
			itoa(*hexValue, 16, hexBuffer);
			printString("0x");
			printString(hexBuffer);
			nextParamShift++;
			currentChar++;
		}else if (strncmp(currentChar, "%s", 2) == 0) {
			char** string = (void*) &format + nextParamShift * STACK_JUMP;
			printString(*string);
			nextParamShift++;
			currentChar++;
		}else if (strncmp(currentChar, "%c", 2) == 0){
			char* character = (void*) &format + nextParamShift * STACK_JUMP;
			printChar(*character);
			nextParamShift++;
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
