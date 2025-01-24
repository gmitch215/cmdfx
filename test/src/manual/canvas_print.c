#include <stdio.h>

#include "cmdfx/canvas.h"
#include "../test.h"

int main() {
    int r = 0;

    Canvas_clearScreen();

    int x = Canvas_getWidth() / 2;
    int y = Canvas_getHeight() / 2;

    int cx = Canvas_getCursorX();
    int cy = Canvas_getCursorY();

    Canvas_hideCursor();

    Canvas_setForeground(0xFF0000);
    Canvas_ellipse(x, y, 12, 6, '$');
    Canvas_resetFormat(x, y);

    Canvas_enableUnderline();
    Canvas_enableBold();
    Canvas_drawText(x - 5, y - 1, "Hello, World!");
    Canvas_disableBold();
    Canvas_disableUnderline();

    Canvas_enableDim();
    Canvas_drawAsciiText(x / 6, y + 1, '%', "ABCDEF");
    Canvas_disableDim();

    Canvas_showCursor();
    Canvas_setCursor(cx, cy);

    return r;
}
