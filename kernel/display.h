#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "../common/types.h"
#include "../common/const.h"
#include "x86.h"
#include "minStdLib.h"
#include "vga_colors.h"
#include "cursor.h"

#ifndef TAB_SIZE
#define TAB_SIZE 4
#endif

#ifndef ZERO_CHAR_VALUE
#define ZERO_CHAR_VALUE 48
#endif

#ifndef VGA_MEMORY
#define VGA_MEMORY (void*) 0xB8000
#endif

#ifndef DISPLAY_SIZE_BYTES
#define DISPLAY_SIZE_BYTES 4000
#endif

void display_init();

void display_clear();

void display_clear_zone(cursor_position_t start_coordinate, int count);

void set_bg_color(uint8_t color);

void set_fg_color(uint8_t color);

void set_colors(uint8_t colors);

uint8_t get_colors();

void print_char(char to_print);

void print_string(char* to_print);

void display_printf(char* format, ...);

void itoa(int value, int base, char* buffer);

void scroll_screen();

#endif /* end of include guard: _DISPLAY_H_ */
