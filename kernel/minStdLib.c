#include "minStdLib.h"

void *memset(void *dst, int value, uint count){
	//TODO actually this writes an int, so 4 bytes, so this should be used to clear screen
	if (count) {
		char* d = dst;
		do {
			*d++ = value;
		} while(--count);
	}
	return dst;
}

void *memcpy(void *dst, void *src, uint count){
	char* d = dst;
	char* s = src;
	for (uint i = 0; i < count; i++) {
		d[i] = s[i];
	}
	return dst;
}

int strncmp(const char *p, const char *q, uint n){
	for (uint i = 0; i < n; i++) {
		if (p[i] < q[i]) {
			return -1;
		}else if (p[i] > q[i]){
			return 1;
		}
	}
	return 0;
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
