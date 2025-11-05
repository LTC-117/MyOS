#ifndef __KSTRING_H__
#define __KSTRING_H__


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


int kstrlen(const char *str, int max);
bool kisdigit(char c);
int kctoi(char c);
int kstrtoi(char *str);


#endif
