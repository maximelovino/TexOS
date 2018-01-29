/**
 * Keyboard source file
 * @file 	keyboard.c
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 21, 2017
 */

#include "keyboard.h"

static int buffer[KEYBOARD_BUFFER_SIZE];
static uint8_t current_read_index = 0;
static uint8_t current_write_index = 0;
static uint8_t buffer_count = 0;

static bool left_shift_down = 0;
static bool right_shift_down = 0;

static int values[] = {
		UNKNOWN_KEY, ESCAPE_CHAR, '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'0', APOSTROPHE_CHAR, '^', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'z',
		'u', 'i', 'o', 'p', E_GRAVE_CHAR, UNKNOWN_KEY, '\n', UNKNOWN_KEY, 'a',
		's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', E_ACUTE_CHAR, A_GRAVE_CHAR,
		DEGREE_SIGN, UNKNOWN_KEY, '$', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',',
		'.', '-', UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, ' ', UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, '<'};

static int shifted_values[] = {
		UNKNOWN_KEY, ESCAPE_CHAR, '+', '"', '*', C_CEDILLA_CHAR, '%', '&', '/',
		'(', ')', '=', '?', '`', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Z', 'U',
		'I', 'O', 'P', U_DIAERESIS_CHAR, '!', '\n', UNKNOWN_KEY, 'A', 'S', 'D',
		'F', 'G', 'H', 'J', 'K', 'L', O_DIAERESIS_CHAR, A_DIAERESIS_CHAR,
		UNKNOWN_KEY, UNKNOWN_KEY, POUND_SIGN, 'Y', 'X', 'C', 'V', 'B', 'N', 'M',
		';', ':', '_', UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, ' ', UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY, UNKNOWN_KEY,
		UNKNOWN_KEY, UNKNOWN_KEY, '>'};

void keyboard_handler() {
	char state = inb(KEYBOARD_STATE_PORT);

	if ((state & 0x1)) {
		//if bit 0 of state is 1, buffer is full, we can read
		uint8_t scan_code = inb(KEYBOARD_DATA_PORT);

		if ((scan_code >> 7)) {
			//key up, break code, just used for shift detection
			switch (scan_code) {
				case LEFT_SHIFT_BREAK_CODE:
					left_shift_down = false;
					break;
				case RIGHT_SHIFT_BREAK_CODE:
					right_shift_down = false;
					break;
				default:
					break;
			}
		} else {
			//key down, make code
			switch (scan_code) {
				case LEFT_SHIFT_MAKE_CODE:
					left_shift_down = true;
					break;
				case RIGHT_SHIFT_MAKE_CODE:
					right_shift_down = true;
					break;
				case UP_ARROW_KEY:
					shift_cursor(0, -1);
					break;
				case DOWN_ARROW_KEY:
					shift_cursor(0, 1);
					break;
				case LEFT_ARROW_KEY:
					shift_cursor(-1, 0);
					break;
				case RIGHT_ARROW_KEY:
					shift_cursor(1, 0);
					break;
				default:
					if (buffer_count == KEYBOARD_BUFFER_SIZE) {
						display_printf("\nBUFFER FULL\n");
					} else {
						if (scan_code > LAST_SUPPORTED_CHAR_MAKE_CODE) {
							buffer[current_write_index++] = UNKNOWN_KEY;
						} else {
							buffer[current_write_index++] = (right_shift_down ||
															 left_shift_down)
															? shifted_values[scan_code]
															: values[scan_code];
						}
						current_write_index %= KEYBOARD_BUFFER_SIZE;
						buffer_count++;
					}
					break;
			}
		}
	}
}

int getc() {
	while (buffer_count == 0) {}

	int character = buffer[current_read_index++];
	current_read_index %= KEYBOARD_BUFFER_SIZE;
	buffer_count--;
	return character;
}

int keypressed() {
	return buffer_count != 0;
}

