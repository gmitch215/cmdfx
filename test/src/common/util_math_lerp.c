#include <stdio.h>

#include "cmdfx/core/util.h"
#include "../test.h"

int main() {
    int r = 0;

    int n1 = lerp_i(0, 10, 0.5);
    r |= assert(n1 == 5);

    int n2 = lerp_i(0, 10, 0.25);
    r |= assert(n2 == 2);

    float f1 = lerp_f(0.0f, 10.0f, 0.5f);
    r |= assert(f1 == 5.0f);

    float f2 = lerp_f(0.0f, 10.0f, 0.25f);
    r |= assert(f2 == 2.5f);

    double d1 = lerp_d(0.0, 10.0, 0.5);
    r |= assert(d1 == 5.0);

    double d2 = lerp_d(0.0, 10.0, 0.25);
    r |= assert(d2 == 2.5);

    return r;
}