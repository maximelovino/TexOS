/**
 * Display header file
 * @file 	display.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 3, 2017
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "../common/types.h"
#include "../common/const.h"
#include "x86.h"
#include "../common/min_std_lib.h"
#include "../common/vga_colors.h"
#include "cursor.h"

#ifndef TAB_SIZE
#define TAB_SIZE 4
#endif

#ifndef VGA_MEMORY
#define VGA_MEMORY (void*) 0xB8000
#endif

#ifndef DISPLAY_SIZE_BYTES
#define DISPLAY_SIZE_BYTES 4000
#endif

/**
 * This function initializes the display by clearing
 * the whole screen (according to the current colors, by default: black background, white text),
 * setting the cursor to the top left corner and enabling it.
 */
extern void display_init();

/**
 * This function clears the whole screen according to the current colors
 */
extern void display_clear();

/**
 * Function to clear part of the screen according to current colors
 * @param start_coordinate A position structure defining the start of the zone to clear
 * @param count The number of characters to clear (by character, we mean a cell of the screen)
 */
extern void display_clear_zone(cursor_position_t start_coordinate, int count);

/**
 * Function to set the background color
 * @param color The new color to set (@see vga_colors.h)
 */
extern void set_bg_color(uint8_t color);

/**
 * Function to set the foreground color
 * @param color The new color to set (@see vga_colors.h)
 */
extern void set_fg_color(uint8_t color);

/**
 * @return The background color currently set
 */
extern uint8_t get_bg_color();

/**
 * @return The foreground color currently set
 */
extern uint8_t get_fg_color();

/**
 * Function to display a character on screen at the current cursor position
 * @param to_print The character to display
 */
extern void print_char(char to_print);

/**
 * Function to display a string on screen at the current cursor position
 * @param to_print The string to display
 */
extern void print_string(char* to_print);

/**
 * Function to display a string on screen at the current cursor position with parameters
 * and variables, using variable arguments count with a syntax similar to printf function from stdio.h
 * Supported format arguments are:
 * - %x to display an HEX value
 * - %d to display a decimal value
 * - %c to display a character
 * - %s to display a string
 * @param format The format of the string
 * @param ... The additional arguments to match the formatted parameters
 */
extern void display_printf(char* format, ...);

/**
 * Function to scroll the screen
 * Implemented by copying the VRAM from 2nd line to end to the first line, and clearing last line
 */
extern void scroll_screen();

/**
 * Function to get a position in the VRAM from a cursor position structure
 * @param position A position structure defining the position we want to get
 * @return A pointer to the VRAM for that coordinate
 */
extern void* get_vram_pointer(cursor_position_t position);


#endif /* end of include guard: _DISPLAY_H_ */
