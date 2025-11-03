#include "string.h"


int strlen(const char *str)
{
    int i = 0;

    while (*str != 0) {
        i++;
        str += 1;
    }

    return i;
}
