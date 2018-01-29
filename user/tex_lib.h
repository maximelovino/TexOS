/**
 * Tex Lib header file
 *
 * @brief	This is the header file for the user applications system library
 * @file 	tex_lib.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	January 29, 2018
 */

#ifndef _TEX_LIB_H_
#define _TEX_LIB_H_

#include "../common/types.h"
#include "../common/tex_fs.h"
#include "../common/min_std_lib.h"

//TODO function headers comments

/**
 * Opens a file
 * @param filename The filename to open
 * @return A file descriptor (>= 0) in case of success, -1 otherwise
 */
extern int file_open(char* filename);

/**
 * Closes a file
 * @param fd The file descriptor to close
 */
extern void file_close(int fd);

/**
 * Reads a file
 * @param fd The file descriptor to read
 * @param buf The allocated buffer in which to read
 * @param count The number of bytes to read
 * @return The number of bytes read, 0 if the file was over, -1 in case of error
 */
extern int file_read(int fd, void* buf, uint count);

/**
 * Stat of a file
 * @param filename The filename to stat
 * @param stat The stat structure in which to write
 * @return 0 in case of success, -1 otherwise
 */
extern int file_stat(char* filename, stat_t* stat);

/**
 * Gets a file iterator
 * @return A file iterator
 */
extern file_iterator_t file_iterator();

/**
 * Function to check if there is a next file in iterator
 * @param it The iterator
 * @return If there is a next file
 */
extern bool file_has_next(file_iterator_t* it);

/**
 * Gets the name of the next file
 * @param filename The buffer in which to put the name of the next file
 * @param iterator The file iterator
 */
extern void file_next(char* filename, file_iterator_t* it);

/**
 * Executes a program
 * @param filename The name of the program to execute
 * @return A status code
 */
extern int exec(char* filename);

/**
 * Exits the current program
 */
extern void exit();

/**
 * Displays a char on screen
 * @param c The char to display
 */
extern void putc(char c);

/**
 * Displays a string on screen
 * @param string The string to display
 */
extern void puts(char* str);

/**
 * Function to display a string on screen at the current cursor position with parameters
 * and variables, using variable arguments count with a syntax similar to printf function from stdio.h
 * Supported format arguments are:
 * - %x to display an HEX value
 * - %d to display a decimal value
 * - %c to display a character
 * - %s to display a string
 * @param fmt The format of the string
 * @param ... The additional arguments to match the formatted parameters
 */
extern void printf(char* fmt, ...);

/**
 * Blocking, gets the pressed character
 * @return The character pressed
 */
extern int getc();

/**
 * Function to check if a key is pressed, non-blocking
 * @return If a key is pressed
 */
extern int keypressed();

/**
 * Function to set the seed of the random
 * @param seed The seed to set
 */
extern void srand(uint seed);

/**
 * Function to get a random int between 0 and 65536
 * @return A random int
 */
extern uint rand();

/**
 * Function to sleep for a given time in milliseconds
 * @param ms The time to sleep in milliseconds
 */
extern void sleep(uint ms);

/**
 * Function to get the timer ticks
 * @return The timer ticks
 */
extern uint get_ticks();

/**
 * Function to check if a strings start with a pattern
 * @param pattern The pattern to check
 * @param string The string to check against
 * @return If the strings starts with the pattern
 */
extern bool starts_with(char* pattern, char* string);

/**
 * Function to remove leading and trailing whitespaces from string
 * @param string The string to remove whitespace from
 * @return The new string
 */
extern char* trim(char* string);

#endif
