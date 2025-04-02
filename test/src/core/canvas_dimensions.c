#include <stdio.h>

#include "cmdfx/core/canvas.h"
#include "../test.h"

int main() {
    Canvas_clearScreen();
    
    int r = 0;

    r |= assertGreaterThan(Canvas_getWidth(), 0);
    r |= assertGreaterThan(Canvas_getHeight(), 0);

    return 0;
}