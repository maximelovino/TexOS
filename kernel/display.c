/**
 * Display source file
 * @file 	display.c
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 3, 2017
 */

#include "display.h"

/**
 * Function to do a new line
 * Clears remainder of current line and puts cursor in first col of next line
 */
static void new_line();

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
	display_clear_zone(start, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}

void display_clear_zone(cursor_position_t start_coordinate, int count) {
	void* start_position = get_vram_pointer(start_coordinate);

	for (int i = 0; i < count && ((start_position + 2 * i + 1) <
								  (VGA_MEMORY + DISPLAY_SIZE_BYTES)); i++) {
		//even bytes are null chars
		memset((start_position + 2 * i), 0, 1);
		//odd bytes are current_color
		memset((start_position + 2 * i + 1), current_color, 1);
	}
}

void set_bg_color(uint8_t color) {
	current_color =
			(current_color & (uint8_t) 0x0F) | (color << 4 & (uint8_t) 0xF0);
}

void set_fg_color(uint8_t color) {
	current_color = (current_color & (uint8_t) 0xF0) | (color & (uint8_t) 0x0F);
}

uint8_t get_bg_color() {
	return (uint8_t) ((current_color >> 4) & 0x0F);
}

uint8_t get_fg_color() {
	return (uint8_t) (current_color & 0x0F);
}

void new_line() {
	cursor_position_t pos = get_cursor_position();
	display_clear_zone(pos, DISPLAY_WIDTH - pos.x);
	shift_cursor(-1 * pos.x, 1);
}

void print_char(char to_print) {
	cursor_position_t cursor = get_cursor_position();
	if (to_print == '\b') {
		move_cursor(-1);
		print_char(0);
		move_cursor(-1);
		return;
	}
	if (convert_2d_to_1d_position(cursor) >= (DISPLAY_HEIGHT * DISPLAY_WIDTH)) {
		scroll_screen();
		cursor.x = 0;
		cursor.y = DISPLAY_HEIGHT - 1;
		set_cursor_position(cursor);
	}
	if (to_print == '\n') {
		new_line();
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
	move_cursor(1);
}

void print_string(char* to_print) {
	int i = 0;
	while (to_print[i]) {
		print_char(to_print[i]);
		i++;
	}
}

void display_printf(char* format, ...) {
	char* current_char = format;
	int next_param_shift = 1;
	while (*current_char) {
		if (strncmp(current_char, "%d", 2) == 0) {
			int* value = (void*) &format + next_param_shift * STACK_JUMP;
			char buffer[ITOA_BUFFER_SIZE] = {0};
			itoa(*value, false, buffer);
			print_string(buffer);
			next_param_shift++;
			current_char++;
		} else if (strncmp(current_char, "%x", 2) == 0) {
			int* hexValue = (void*) &format + next_param_shift * STACK_JUMP;
			char hexBuffer[ITOA_BUFFER_SIZE] = {0};
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
	memcpy(VGA_MEMORY, VGA_MEMORY + (DISPLAY_WIDTH * 2),
		   DISPLAY_SIZE_BYTES - (DISPLAY_WIDTH * 2));
	// We clear from start of last line for a whole line
	cursor_position_t start_clear;
	start_clear.x = 0;
	start_clear.y = DISPLAY_HEIGHT - 1;
	display_clear_zone(start_clear, DISPLAY_WIDTH);
}

void* get_vram_pointer(cursor_position_t position) {
	return VGA_MEMORY + convert_2d_to_1d_position(position) * 2;
}
