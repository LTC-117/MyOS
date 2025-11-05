#ifndef __MEMORY_H__
#define __MEMORY_H__


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


void *memset(void *ptr, int c, size_t size);
int memcmp(void *s1, void *s2, int count);

#endif
