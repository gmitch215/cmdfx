#include <stdio.h>

#include <CoreGraphics/CoreGraphics.h>

#include "cmdfx/screen.h"

// Screen API

void Screen_getSize(int* width, int* height) {
    if (width == 0) return;
    if (height == 0) return;

    CGDirectDisplayID display = CGMainDisplayID();
    *width = CGDisplayPixelsWide(display);
    *height = CGDisplayPixelsHigh(display);
}