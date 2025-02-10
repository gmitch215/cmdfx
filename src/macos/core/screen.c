#include <stdio.h>

#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>

#include "cmdfx/core/screen.h"

// Screen API

void Screen_getSize(int* width, int* height) {
    if (width == 0) return;
    if (height == 0) return;

    CGDirectDisplayID display = CGMainDisplayID();
    *width = CGDisplayPixelsWide(display);
    *height = CGDisplayPixelsHigh(display);
}

void Screen_getMousePos(int* x, int* y) {
    if (x == 0) return;
    if (y == 0) return;

    CGEventRef event = CGEventCreate(0);
    CGPoint point = CGEventGetLocation(event);

    *x = point.x;
    *y = point.y;

    CFRelease(event);
}