/**
 * Keyboard header file
 * @file 	keyboard.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 21, 2017
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "pio_asm.h"
#include "display.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATE_PORT 0x64

#define LEFT_SHIFT_MAKE_CODE 0x2A
#define LEFT_SHIFT_BREAK_CODE 0xAA

#define RIGHT_SHIFT_MAKE_CODE 0x36
#define RIGHT_SHIFT_BREAK_CODE 0xB6

#define LAST_SUPPORTED_CHAR_MAKE_CODE 86

#define UNKNOWN_KEY (-1)
#define APOSTROPHE_CHAR 39
#define E_ACUTE_CHAR 130
#define E_GRAVE_CHAR 138
#define A_GRAVE_CHAR 133
#define DEGREE_SIGN 248
#define C_CEDILLA_CHAR 135
#define U_DIAERESIS_CHAR 129
#define O_DIAERESIS_CHAR 148
#define A_DIAERESIS_CHAR 132
#define POUND_SIGN 156


//ARROW KEYS DEFINITION

#define LEFT_ARROW_KEY 75
#define UP_ARROW_KEY 72
#define DOWN_ARROW_KEY 80
#define RIGHT_ARROW_KEY 77

#define KEYBOARD_BUFFER_SIZE 100

/**
 * Function called when a key is pressed, dispatched from irq_handler
 * This reads from the keyboard and stores in the buffer
 */
extern void keyboard_handler();

/**
 * Blocking call to read a character from the buffer
 * @return The value of the character read from the buffer
 */
extern int getc();

/**
 * Non-blocking function to check if a key was pressed
 * (if there's something to read in the buffer)
 * @return If a key was pressed or not
 */
extern int keypressed();

#endif
