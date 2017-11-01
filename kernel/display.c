#include "display.h"

static uint8_t current_color = (COLOR_BLACK << 4) | COLOR_WHITE;

void display_init() {
	display_clear();
	set_cursor_to_origin();
	set_cursor_default_look();
	enable_cursor();
}

void display_clear() {
	cursor_position_t start;
	start.x = 0;
	start.y = 0;
	display_clear_zone(start, DISPLAY_SIZE_BYTES);
}

void display_clear_zone(cursor_position_t start_coordinate, int count) {
	//TODO we should decide if we always clear white on black or not
	void* start_position = get_vram_pointer(start_coordinate);

	for (int i = 0; i < count; i++) {
		if (i % 2 == 0) {
			//even bytes are null chars
			memset((start_position + i), 0, 1);
		} else {
			//odd bytes are current_color
			memset((start_position + i), current_color, 1);
		}
	}
}

void set_colors(uint8_t colors) {
	current_color = colors;
}

//TODO separate this in two distinct functions
uint8_t get_colors() {
	return current_color;
}

void print_char(char to_print) {
	cursor_position_t cursor = get_cursor_position();
	if (convert_2d_to_1d_position(cursor) >= (DISPLAY_HEIGHT * DISPLAY_WIDTH)) {
		scroll_screen();
		shift_cursor(0, -1);
		cursor = get_cursor_position();
	}
	if (to_print == '\n') {
		carriage_return();
		return;
	}
	if (to_print == '\t') {
		for (char i = 0; i < TAB_SIZE; i++) {
			print_char(' ');
		}
		return;
	}

	void* address = get_vram_pointer(cursor);
	memset(address, to_print, 1); // first the character
	memset(address + 1, current_color, 1); //then the current_color
	increment_cursor();
}

void print_string(char* to_print) {
	int i = 0;
	while (to_print[i]) {
		print_char(to_print[i]);
		i++;
	}
}

void itoa(int value, bool hex, char* buffer) {
	//TODO temporary hack => do it better later

	buffer[0] = 0; //This is the null terminator
	int current = 0;
	if (value == 0) {
		buffer[0] = '0';
		return;
	}

	int base = hex ? 16 : 10;

	while (value) {
		char rem = value % base;
		char to_append;
		if (rem >= 10) {
			to_append = 'A' + rem - 10;
		} else {
			to_append = '0' + rem;
		}
		buffer[current] = to_append;
		value /= base;
		current++;
	}

	char tempBuffer[100];
	for (int i = 0; i < current; i++) {
		tempBuffer[i] = buffer[current - i - 1];
	}
	memcpy(buffer, tempBuffer, current);
}

void display_printf(char* format, ...) {
	char* current_char = format;
	int next_param_shift = 1;
	while (*current_char) {
		if (strncmp(current_char, "%d", 2) == 0) {
			int* value = (void*) &format + next_param_shift * STACK_JUMP;
			char buffer[100] = {0};
			itoa(*value, false, buffer);
			print_string(buffer);
			next_param_shift++;
			current_char++;
		} else if (strncmp(current_char, "%x", 2) == 0) {
			int* hexValue = (void*) &format + next_param_shift * STACK_JUMP;
			char hexBuffer[100] = {0};
			itoa(*hexValue, true, hexBuffer);
			print_string("0x");
			print_string(hexBuffer);
			next_param_shift++;
			current_char++;
		} else if (strncmp(current_char, "%s", 2) == 0) {
			char** string = (void*) &format + next_param_shift * STACK_JUMP;
			print_string(*string);
			next_param_shift++;
			current_char++;
		} else if (strncmp(current_char, "%c", 2) == 0) {
			char* character = (void*) &format + next_param_shift * STACK_JUMP;
			print_char(*character);
			next_param_shift++;
			current_char++;
		} else {
			print_char(*current_char);
		}
		current_char++;
	}
}

void scroll_screen() {
	memcpy(VGA_MEMORY, VGA_MEMORY + (DISPLAY_WIDTH * 2), DISPLAY_SIZE_BYTES - (DISPLAY_WIDTH * 2));
	// We clear from start of last line for a whole line
	cursor_position_t start_clear;
	start_clear.x = 0;
	start_clear.y = DISPLAY_HEIGHT - 1;
	display_clear_zone(start_clear, DISPLAY_WIDTH * 2);
}

void set_bg_color(uint8_t color) {
	set_colors((current_color & (uint8_t) 0x0F) | (color << 4 & (uint8_t) 0xF0));
}

void set_fg_color(uint8_t color) {
	set_colors((get_colors() & (uint8_t) 0xF0) | (color & (uint8_t) 0x0F));
}

void* get_vram_pointer(cursor_position_t position) {
	return VGA_MEMORY + convert_2d_to_1d_position(position) * 2;
}
