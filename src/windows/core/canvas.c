#include "cmdfx/core/canvas.h"

#include "common/core/curses_backend.h"

// the curses backend owns all terminal I/O; the old console api cursor calls
// and system("cls") are gone (curses tracks the cursor via getyx/move)

int Canvas_getCursorX() {
    int x, y;
    CmdFX_curses_getCursor(&x, &y);
    return x;
}

int Canvas_getCursorY() {
    int x, y;
    CmdFX_curses_getCursor(&x, &y);
    return y;
}

void Canvas_setCursor(int x, int y) {
    if (x < 0) return;
    if (y < 0) return;

    CmdFX_curses_moveCursor(x, y);
}

void Canvas_hideCursor() {
    CmdFX_curses_setCursorVisible(0);
}

void Canvas_showCursor() {
    CmdFX_curses_setCursorVisible(1);
}

int Canvas_isCursorVisible() {
    return CmdFX_curses_isCursorVisible();
}

void Canvas_clearScreen() {
    CmdFX_curses_clear();
}
