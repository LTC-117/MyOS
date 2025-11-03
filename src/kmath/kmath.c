#include "kmath.h"


int kpow(int base, int pow)
{
    int power = 1;

    for (int i = 0; i < pow; i++) {
        power *= base;
    }

    return power;
}
