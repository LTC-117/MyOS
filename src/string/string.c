#include "string.h"

#include "../kmath/kmath.h"


int strlen(const char *str)
{
    int i = 0;

    while (*str != 0) {
        i++;
        str += 1;
    }

    return i;
}


int ctoi(char c)
{
    return c - '0';
}


int strtoi(char *str)
{
    int len = strlen(str);
    int num = 0;

    for (int i = 0; i <= len; i++) {
        num += (str[i] - '0') * (kpow(10, len));
    }

    return num;
}
