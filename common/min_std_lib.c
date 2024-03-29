/**
 * Minimal stdlib source file
 * @file 	min_std_lib.c
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 3, 2017
 */

#include <ctype.h>
#include "min_std_lib.h"

void* memset(void* dst, int value, uint count) {
	unsigned char val = (unsigned char) value;
	char* d = dst;
	while (count--) {
		*d++ = val;
	}
	return dst;
}

void* memcpy(void* dst, void* src, uint count) {
	char* d = dst;
	char* s = src;
	for (uint i = 0; i < count; i++) {
		d[i] = s[i];
	}
	return dst;
}

int strncmp(const char* p, const char* q, uint n) {
	for (uint i = 0; i < n; i++) {
		if (p[i] < q[i]) {
			return -1;
		} else if (p[i] > q[i]) {
			return 1;
		}
	}
	return 0;
}

void itoa(int value, bool hex, char* buffer) {

	buffer[0] = 0; //This is the null terminator
	uint current = 0;
	bool negative = false;
	if (value == 0) {
		buffer[0] = '0';
		return;
	}

	if (value < 0) {
		negative = true;
		value = value * -1;
	}

	int base = hex ? 16 : 10;

	while (value) {
		char rem = (char) (value % base);
		char to_append;
		if (rem >= 10) {
			to_append = (char) ('A' + rem - 10);
		} else {
			to_append = (char) ('0' + rem);
		}
		buffer[current] = to_append;
		value /= base;
		current++;
	}

	char tempBuffer[ITOA_BUFFER_SIZE] = {0};
	char* bufferToCopy = &tempBuffer[0];
	if (negative) {
		tempBuffer[0] = '-';
		bufferToCopy = &tempBuffer[1];
	}
	for (uint i = 0; i < current; i++) {
		bufferToCopy[i] = buffer[current - i - 1];
	}
	memcpy(buffer, tempBuffer, ITOA_BUFFER_SIZE);
}

int strlen(char* string) {
	int i = 0;
	char* str = string;
	while (*str) {
		i++;
		str++;
	}
	return i;
}

int strcmp(char* p, char* q) {
	while (*p) {
		if (*p > *q) {
			return 1;
		} else if (*p < *q) {
			return -1;
		}
		p++;
		q++;
	}
	return *q ? -1 : 0;
}


int atoi(char* string) {
	int result = 0;
	unsigned int digit;
	bool negative;

	if (*string == '-') {
		negative = true;
		string += 1;
	} else {
		negative = false;
		if (*string == '+') {
			string += 1;
		}
	}
	do {
		digit = *string - '0';
		if (digit > 9) {
			break;
		}
		result = (10 * result) + digit;
	} while (string += 1);

	if (negative) {
		return -result;
	}
	return result;
}