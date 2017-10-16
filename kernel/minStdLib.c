#include "minStdLib.h"

void *memset(void *dst, int value, uint count){
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
	return 0;
}
