#include "string.h"

#include "../kmath/kmath.h"


int kstrlen(const char *str)
{
    int i = 0;

    while (*str != 0) {
        i++;
        str += 1;
    }

    return i;
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
        num += (str[i] - '0') * (kpow(10, len));
    }

    return num;
}
