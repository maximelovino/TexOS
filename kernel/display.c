#include "display.h"

static uint8_t color = 0x0F;
//TODO Store cursor position here

void display_init(){
	cursor_position_t newPos;
	newPos.x = 0;
	newPos.y = 0;
	display_clear();
	setCursorPosition(newPos);
}

void display_clear(){
	memset((void*) VGA_MEMORY,0,DISPLAY_SIZE_BYTES);
}

void set_colors(uint8_t colors){
	color = colors;
}

uint8_t get_colors(){
	return color;
}

void printChar(char toPrint){
	//TODO if we are writing on an outside cursor, we scroll the screen and move the cursor at the begininng of last line
	cursor_position_t cursor = getCursorPosition();
	uint16_t position_1d = convert_2d_position(cursor);
	void* address = (void*)VGA_MEMORY + (position_1d * 2);
	memset(address, toPrint, 1); // first the character
	memset(address + 1, color, 1); //then the color
	//SHOULD WE MOVE THE CURSOR?
	//TODO if we write on the last cell of the screen, we move the cursor outside
}

void printString(char* toPrint){
	int i = 0;
	while (toPrint[i] ) {
		printChar(toPrint[i]);
	}
}

void setCursorPosition(cursor_position_t position){

}

cursor_position_t getCursorPosition(){
	cursor_position_t position;
	position.x = 0;
	position.y = 0;

	return position;
}

void display_print(char* format, ...){

}

uint16_t convert_2d_position(cursor_position_t cursor){
	return 0;
}

cursor_position_t convert_1d_position(uint16_t position_1d){

}

void scroll_screen(){

}
