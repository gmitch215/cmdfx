#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/canvas.h"

int main() {
    Canvas_clearScreen();

    int r = 0;

    // dimensions are non-negative; 0 means no attached terminal (unbounded)
    r |= assertGreaterThan(Canvas_getWidth(), -1);
    r |= assertGreaterThan(Canvas_getHeight(), -1);

    return r;
}