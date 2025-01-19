#include <stdio.h>
#include "cmdfx/canvas.h"

void Canvas_setChar(int x, int y, char c) {
    int currentColor = Canvas_getColor();
    Canvas_setCursor(x, y);
    putchar(c);
}