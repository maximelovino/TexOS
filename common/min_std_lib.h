/**
 * Minimal stdlib header file
 * @file 	min_std_lib.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	November 3, 2017
 */

#ifndef _MIN_STD_LIB_H_
#define _MIN_STD_LIB_H_
#include "types.h"

#define ITOA_BUFFER_SIZE 16

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a < b ? a : b)

/**
 * This function writes count bytes of value
 * (converted to an unsigned char) to the memory at dst.
 * @param dst The pointer to the memory we want to set
 * @param value The value to set (int that will be converted to unsigned char)
 * @param count The number of bytes to write
 * @return A pointer to the memory we set
 */
extern void* memset(void* dst, int value, uint count);

/**
 * This functions copies count bytes of memory from the src to the dst
 * @param dst The memory we want to copy to
 * @param src The memory we want to copy from
 * @param count The number of bytes to copy
 * @return A pointer to the memory we set
 */
extern void* memcpy(void* dst, void* src, uint count);

/**
 * Function to compare two strings on n bytes
 * @param p The first string
 * @param q The second string
 * @param n The number of bytes to compare
 * @return -1, 0 or 1 if the p is respectively less, equal or bigger than q
 */
extern int strncmp(const char* p, const char* q, uint n);

/**
 * Function to represent an int in a string
 * @param value The value to convert
 * @param hex If we want to convert in hexadecimal notation (decimal otherwise)
 * @param buffer The buffer to write the converted int to
 */
extern void itoa(int value, bool hex, char* buffer);

/**
 * Function to get the length of a string
 * @param string The string we want to get the length of
 * @return The length of that string
 */
extern int strlen(char* string);

/**
 * Function to compare two strings until the end of the first one
 * @param p	The first string
 * @param q	The second string
 * @return -1, 0, or 1 if p is respectively less, equal or bigger than q
 */
extern int strcmp(char* p, char* q);

/**
 * Return int value from a string, string can contain spaces before numbers and other characters after
 * @param string string to be evaluate
 * @return int value
 */
extern int atoi(char *string);

#endif /* end of include guard: _MIN_STD_LIB_H_ */
