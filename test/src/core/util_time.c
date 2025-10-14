#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/util.h"

int main() {
    int r = 0;

    unsigned long start1 = currentTimeMillis();
    sleepMillis(50);
    unsigned long end1 = currentTimeMillis();

    r |= assertGreaterThan(end1 - start1, 49);

    unsigned long long start2 = currentTimeNanos();
    sleepNanos(50);
    unsigned long long end2 = currentTimeNanos();

    r |= assertGreaterThan(end2 - start2, 49);

    return r;
}