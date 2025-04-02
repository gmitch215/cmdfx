#include <stdio.h>

#include "cmdfx/core/screen.h"
#include "../test.h"

int main() {
    int r = 0;

    // Window API

    Window_setTitle("Test Window");
    r |= assertStringsMatch(Window_getTitle(), "Test Window");

    Window_setTitle("Another Test Window");
    r |= assertStringsMatch(Window_getTitle(), "Another Test Window");

    int width = -1, height = -1;
    Window_getSize(&width, &height);

    r |= assertGreaterThan(width, -1);
    r |= assertGreaterThan(height, -1);

    Window_setSize(80, 24);

    int width2 = -1, height2 = -1;
    Window_getSize(&width2, &height2);
    
    // Resize may not work in headless
    r |= assertGreaterThan(width2, -1);
    r |= assertGreaterThan(height2, -1);

    // Screen API

    int width3 = -1, height3 = -1;
    Screen_getSize(&width3, &height3);

    r |= assertGreaterThan(width3, -1);
    r |= assertGreaterThan(height3, -1);

    int mx = -2, my = -2;
    Screen_getMousePos(&mx, &my);

    printf("Mouse: %d, %d\n", mx, my);
    r |= assertGreaterThan(mx, -2);
    r |= assertGreaterThan(my, -2);

    return r;
}