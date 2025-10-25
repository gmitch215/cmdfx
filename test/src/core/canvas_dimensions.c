#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/canvas.h"

int main() {
    Canvas_clearScreen();

    int r = 0;

    r |= assertGreaterThan(Canvas_getWidth(), 0);
    r |= assertGreaterThan(Canvas_getHeight(), 0);

    return 0;
}