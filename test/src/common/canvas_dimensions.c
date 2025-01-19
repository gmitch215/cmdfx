#include <stdio.h>

#include "cmdfx/canvas.h"
#include "../test.h"

int main() {
    Canvas_clearScreen();
    
    int r = 0;

    r |= assert(Canvas_getWidth() > 0);
    r |= assert(Canvas_getHeight() > 0);

    return 0;
}