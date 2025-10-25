#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/util.h"

int main() {
    int r = 0;

    int n1 = clamp_i(5, 0, 10);
    r |= assertEquals(n1, 5);

    int n2 = clamp_i(7, 0, 5);
    r |= assertEquals(n2, 5);

    int n3 = clamp_i(-1, 0, 5);
    r |= assertEquals(n3, 0);

    float f1 = clamp_f(5.0f, 0.0f, 10.0f);
    r |= assertFloatEquals(f1, 5.0f);

    float f2 = clamp_f(7.0f, 0.0f, 5.0f);
    r |= assertFloatEquals(f2, 5.0f);

    float f3 = clamp_f(-1.0f, 0.0f, 5.0f);
    r |= assertFloatEquals(f3, 0.0f);

    double d1 = clamp_d(5.0, 0.0, 10.0);
    r |= assertDoubleEquals(d1, 5.0);

    double d2 = clamp_d(7.0, 0.0, 5.0);
    r |= assertDoubleEquals(d2, 5.0);

    double d3 = clamp_d(-1.0, 0.0, 5.0);
    r |= assertDoubleEquals(d3, 0.0);

    return r;
}