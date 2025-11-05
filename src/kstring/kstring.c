#include "kstring.h"

#include "../kmath/kmath.h"

// Correction


int kstrlen(const char *str)
{
    int i = 0;

    while (*str != 0) {
        i++;
        str += 1;
    }

    return i;
}


bool kisdigit(char c)
{
    return (c >= 48) && (c <= 57);
}


int kctoi(char c)
{
    return c - '0';
}


int kstrtoi(char *str)
{
    int len = kstrlen(str);
    int num = 0;

    for (int i = 0; i <= len; i++) {
        num += kctoi(str[i]) * (kpow(10, len));
    }

    return num;
}
