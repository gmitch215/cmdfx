#include <stdlib.h>

#include "../test.h"
#include "cmdfx/core/builder.h"

static void freeStr(char*** a, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) free(a[y][x]);
        free(a[y]);
    }
    free(a);
}

int main() {
    int r = 0;

    // createFilled with a foreground RGB color
    char*** fg = String2DBuilder_createFilledForeground(3, 2, 0xFF8800);
    r |= assertNotNull(fg);
    r |= assertStringsMatch(fg[0][0], "\033[38;2;255;136;0m");
    r |= assertStringsMatch(fg[1][2], "\033[38;2;255;136;0m");

    // overlaying a background composes both codes onto each cell
    r |= assertEquals(String2DBuilder_setBackgroundAll(fg, 0x000010), 0);
    r |= assertStringsMatch(fg[0][0], "\033[38;2;255;136;0m\033[48;2;0;0;16m");
    freeStr(fg, 3, 2);

    // 256-color and 8/16-color creators
    char*** c256 = String2DBuilder_createFilledForeground256(2, 2, 240);
    r |= assertNotNull(c256);
    r |= assertStringsMatch(c256[0][0], "\033[38;5;240m");
    freeStr(c256, 2, 2);

    char*** c8 = String2DBuilder_createFilledColor8(2, 2, 41);
    r |= assertNotNull(c8);
    r |= assertStringsMatch(c8[0][0], "\033[41m");
    freeStr(c8, 2, 2);

    // region fill only touches the requested rectangle
    char*** reg = String2DBuilder_createFilled(4, 4, " ");
    r |= assertEquals(
        String2DBuilder_setForeground(reg, 1, 1, 2, 2, 0x00FF00), 0
    );
    r |= assertStringsMatch(reg[1][1], " \033[38;2;0;255;0m");
    r |= assertStringsMatch(reg[0][0], " ");
    // a region that overflows the array is rejected
    r |= assertEquals(
        String2DBuilder_setForeground(reg, 3, 3, 4, 4, 0xFFFFFF), -1
    );
    freeStr(reg, 4, 4);

    // invalid colors and arrays are rejected
    r |= assertNull(String2DBuilder_createFilledForeground(2, 2, -1));
    r |= assertNull(String2DBuilder_createFilledForeground256(2, 2, 999));
    r |= assertNull(String2DBuilder_createFilledColor8(2, 2, 10));
    r |= assertEquals(String2DBuilder_setColor8All(0, 41), -1);

    return r;
}
