#include <stdio.h>

#include "cmdfx/core/util.h"
#include "../test.h"

int main() {
    int r = 0;

    unsigned long start1 = currentTimeMillis();
    sleepMillis(50);
    unsigned long end1 = currentTimeMillis();

    r |= assert(end1 - start1 >= 50);

    unsigned long long start2 = currentTimeNanos();
    sleepNanos(50);
    unsigned long long end2 = currentTimeNanos();

    r |= assert(end2 - start2 >= 50);

    return r;
}