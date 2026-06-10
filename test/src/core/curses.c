#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/canvas.h"
#include "cmdfx/core/screen.h"

int main() {
    int r = 0;

    // dimensions are always non-negative (0 when headless / unbounded)
    r |= assertGreaterThan(Canvas_getWidth(), -1);
    r |= assertGreaterThan(Canvas_getHeight(), -1);

    // a pile of drawing ops must never crash, with or without a terminal
    Canvas_clearScreen();
    Canvas_setForeground(0xFF8800);
    Canvas_setBackground(0x001122);
    Canvas_enableBold();
    Canvas_enableUnderline();
    Canvas_setChar(1, 1, 'X');
    Canvas_setAnsi(2, 2, "\033[38;5;160m");
    Canvas_drawText(1, 3, "curses");
    Canvas_hLine(1, 4, 6, '-');
    Canvas_vLine(1, 5, 3, '|');
    Canvas_rect(1, 6, 5, 3, '#');
    Canvas_resetFormat();

    // still sane after drawing
    r |= assertGreaterThan(Canvas_getWidth(), -1);

    // cursor visibility round-trips
    Canvas_hideCursor();
    r |= assertFalse(Canvas_isCursorVisible());
    Canvas_showCursor();
    r |= assertTrue(Canvas_isCursorVisible());

    // window size query is non-negative
    int w = -5, h = -5;
    Window_getSize(&w, &h);
    r |= assertGreaterThan(w, -1);
    r |= assertGreaterThan(h, -1);

    // echo / line-buffer state round-trips through the backend
    Screen_setEchoEnabled(0);
    r |= assertFalse(Screen_isEchoEnabled());
    Screen_setEchoEnabled(1);
    r |= assertTrue(Screen_isEchoEnabled());
    Screen_setLineBuffered(0);
    r |= assertFalse(Screen_isLineBuffered());
    Screen_setLineBuffered(1);
    r |= assertTrue(Screen_isLineBuffered());

    // title round-trips
    Window_setTitle("cmdfx curses test");
    r |= assertStringsMatch(Window_getTitle(), "cmdfx curses test");

    return r;
}
