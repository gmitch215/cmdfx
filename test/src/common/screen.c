#include <stdio.h>
#include <string.h>

#include "cmdfx/core/screen.h"
#include "../test.h"

int main() {
    int r = 0;

    // Window API

    Window_setTitle("Test Window");
    r |= assert(strcmp(Window_getTitle(), "Test Window") == 0);

    Window_setTitle("Another Test Window");
    r |= assert(strcmp(Window_getTitle(), "Another Test Window") == 0);

    int width = -1, height = -1;
    Window_getSize(&width, &height);

    r |= assert(width > -1);
    r |= assert(height > -1);

    Window_setSize(80, 24);

    int width2 = -1, height2 = -1;
    Window_getSize(&width2, &height2);
    
    // Resize may not work in headless
    r |= assert(width2 > -1);
    r |= assert(height2 > -1);

    // Screen API

    int width3 = -1, height3 = -1;
    Screen_getSize(&width3, &height3);

    r |= assert(width3 > -1);
    r |= assert(height3 > -1);

    int mx = -1, my = -1;
    Screen_getMousePos(&mx, &my);

    printf("Mouse: %d, %d\n", mx, my);
    r |= assert(mx > -1);
    r |= assert(my > -1);

    return r;
}