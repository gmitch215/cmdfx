#include <stdio.h>

#include "cmdfx/canvas.h"
#include "../test.h"

int main() {
    int r = 0;

    Canvas_clearScreen();

    int x = Canvas_getWidth() / 2;
    int y = Canvas_getHeight() / 2;

    Canvas_hideCursor();

    Canvas_setForeground(0xFF0000);
    Canvas_ellipse(x, y, 24, 12, '$');
    Canvas_resetFormat(x, y);

    Canvas_drawText(x - 10, y - 1, "Hello, World!");
    Canvas_drawAsciiText(x - 5, y + 20, '%', "ABC");

    Canvas_showCursor();

    return r;
}