#ifndef _MIN_STD_LIB_H_
#define _MIN_STD_LIB_H_
#include "../common/types.h"

void *memset(void *dst, int value, uint count);
void *memcpy(void *dst, void *src, uint count);
int strncmp(const char *p, const char *q, uint n);

void itoa(int value, bool hex, char* buffer);

//TODO code strlen

#endif /* end of include guard: _MIN_STD_LIB_H_ */
