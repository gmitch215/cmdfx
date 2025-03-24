#include <stdio.h>

#include "cmdfx/core/util.h"
#include "../test.h"

int main() {
    int r = 0;

    int n1 = clamp_i(5, 0, 10);
    r |= assert(n1 == 5);

    int n2 = clamp_i(7, 0, 5);
    r |= assert(n2 == 5);

    int n3 = clamp_i(-1, 0, 5);
    r |= assert(n3 == 0);

    float f1 = clamp_f(5.0f, 0.0f, 10.0f);
    r |= assert(f1 == 5.0f);

    float f2 = clamp_f(7.0f, 0.0f, 5.0f);
    r |= assert(f2 == 5.0f);

    float f3 = clamp_f(-1.0f, 0.0f, 5.0f);
    r |= assert(f3 == 0.0f);

    double d1 = clamp_d(5.0, 0.0, 10.0);
    r |= assert(d1 == 5.0);

    double d2 = clamp_d(7.0, 0.0, 5.0);
    r |= assert(d2 == 5.0);

    double d3 = clamp_d(-1.0, 0.0, 5.0);
    r |= assert(d3 == 0.0);

    return r;
}