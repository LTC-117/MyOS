#include "kstring.h"

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


int kstrncmp(char *first, char *second, int size)
{
    int res = 0;

    size = (size > kstrlen(first))
        ? kstrlen(first)
        : (size > kstrlen(second))
        ? kstrlen(second)
        : size;

    for (int i = 0; i < size; i++) {
        res = first - second;

        if (res != 0) break;
    }

    return res;
}


int kstrcmp(char *first, char *second)
{
    int size1 = kstrlen(first);
    int size2 = kstrlen(second);

    if (size1 >= size2) return kstrncmp(first, second, size1);

    return kstrncmp(first, second, size2);
}
