#include "keyboard.h"

static char buffer[10];
static uint8_t current_read_index = 0;
static uint8_t current_write_index = 0;

static bool left_shift_down = 0;
static bool right_shift_down = 0;

//TODO don't forget space and Tab

static uint8_t values[] = {' ', ' ', '1', '2', '3', '4', '5', '6', '7', '8',
						   '9',
						   '0', 39, '^', ' ', ' ', 'q', 'w', 'e', 'r', 't', 'z',
						   'u',
						   'i', 'o', 'p', 138, ' ', '\n', ' ', 'a', 's', 'd',
						   'f',
						   'g', 'h', 'j', 'k', 'l', 130, 133, ' ', ' ', '$',
						   'y',
						   'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', ' ',
						   ' ', ' ', ' '};

static uint8_t shifted_values[] = {' ', ' ', '+', '"', '*', 135, '%', '&', '/',
								   '(',
								   ')',
								   '=', '?', '`', ' ', ' ', 'Q', 'W', 'E', 'R',
								   'T',
								   'Z', 'U',
								   'I', 'O', 'P', 129, '!', '\n', ' ', 'A', 'S',
								   'D',
								   'F',
								   'G', 'H', 'J', 'K', 'L', 'ö', 132, ' ', ' ',
								   156,
								   'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':',
								   '_', ' ', ' ', ' ', ' '};


void keyboard_init() {

}

void keyboard_handler() {
	char state = inb(KEYBOARD_STATE_PORT);

	if ((state & 0x1)) {
		//if bit 0 of state is 1, buffer is full, we can read
		uint8_t scan_code = inb(KEYBOARD_DATA_PORT);

		if ((scan_code >> 7)) {
			//key up, break code
			if (scan_code == LEFT_SHIFT_BREAK_CODE) {
				left_shift_down = false;
			} else if (scan_code == RIGHT_SHIFT_BREAK_CODE) {
				right_shift_down = false;
			} else {
				//if (left_shift_down || right_shift_down)
				//display_printf("SHIFT ");
				//display_printf("UP BREAK CODE %d\n", scan_code);
			}
		} else {
			//key down, make code
			if (scan_code == LEFT_SHIFT_MAKE_CODE) {
				left_shift_down = true;
			} else if (scan_code == RIGHT_SHIFT_MAKE_CODE) {
				right_shift_down = true;
			} else if (scan_code == 1) {
				//TODO REMOVE just for the tests
				display_init();
			} else {
				display_printf("DOWN MAKE CODE %d ", scan_code);
				if (left_shift_down || right_shift_down)
					display_printf("%c\n", shifted_values[scan_code]);
				else
					display_printf("%c\n", values[scan_code]);
			}
		}
	}
}

int getc() {

}

// Non-blocking call. Return 1 if a key is pressed
int keypressed() {

}

