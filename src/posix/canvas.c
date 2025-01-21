#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>

#include "cmdfx/canvas.h"

int _width = 0;
int _height = 0;

int Canvas_getWidth() {
    if (_width != 0) return _width;
    
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        _width = ws.ws_col;
    }

    return _width;
}

int Canvas_getHeight() {
    if (_height != 0) return _height;
    
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        _height = ws.ws_col;
    }
    
    return _height;
}

int Canvas_getCursorX() {
    printf("\033[6n");
    fflush(stdout);

    int x, y;
    if (scanf("\033[%d;%dR", &y, &x) == 2)
        return x;
    
    return -1;
}

int Canvas_getCursorY() {
    printf("\033[6n");
    fflush(stdout);

    int x, y;
    if (scanf("\033[%d;%dR", &y, &x) == 2)
        return y;
    
    return -1;
}

void Canvas_setCursor(int x, int y) {
    int width = Canvas_getWidth();
    int height = Canvas_getHeight();

    if (x < 1 || x > width) return;
    if (y < 1 || y > height) return;

    printf("\033[%d;%dH", y, x);
}

int _cursorShown = 0;

void Canvas_hideCursor() {
    if (!_cursorShown) return;

    printf("\033[?25l");
    _cursorShown = 0;
}

void Canvas_showCursor() {
    if (_cursorShown) return;

    printf("\033[?25h");
    _cursorShown = 1;
}

int Canvas_isCursorVisible() {
    return _cursorShown;
}

void Canvas_clearScreen() {
    system("clear");
}