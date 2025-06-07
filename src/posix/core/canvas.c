#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "cmdfx/core/util.h"
#include "cmdfx/core/canvas.h"

#define _CANVAS_MUTEX 7

int _Canvas_getPos(int *y, int *x) {
    char buf[30] = {0};
    int ret, i, pow;
    char ch;

    *y = 0; *x = 0;

    struct termios term, restore;
    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for (i = 0, ch = 0; ch != 'R'; i++) {
        ret = read(0, &ch, 1);
        if (!ret) {
            tcsetattr(0, TCSANOW, &restore);
            fprintf(stderr, "getpos: error reading response!\n");
            return 1;
        }

        buf[i] = ch;
    }

    if (i < 2) {
        tcsetattr(0, TCSANOW, &restore);
        return 1;
    }

    for(i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
        *x = *x + (buf[i] - '0') * pow;

    for(i--, pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + (buf[i] - '0') * pow;

    tcsetattr(0, TCSANOW, &restore);
    return 0;
}

int Canvas_getCursorX() {
    int x, y;
    if (_Canvas_getPos(&y, &x)) return -1;

    return x;
}

int Canvas_getCursorY() {
    int x, y;
    if (_Canvas_getPos(&y, &x)) return -1;
    
    return y;
}

void Canvas_setCursor(int x, int y) {
    if (x < 0) return;
    if (y < 0) return;

    printf("\033[%d;%dH", y, x);
}

int _cursorShown = 1;

void Canvas_hideCursor() {
    if (!_cursorShown) return;

    printf("\e[?25l");
    _cursorShown = 0;
}

void Canvas_showCursor() {
    if (_cursorShown) return;

    printf("\e[?25h");
    _cursorShown = 1;
}

int Canvas_isCursorVisible() {
    return _cursorShown;
}

void Canvas_clearScreen() {
    system("clear");
}