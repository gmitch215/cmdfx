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

void Canvas_setCursor(int x, int y) {
    int width = Canvas_getWidth();
    int height = Canvas_getHeight();

    if (x < 1 || x > width) return;
    if (y < 1 || y > height) return;

    printf("\033[%d;%dH", y, x);
}

void Canvas_hideCursor() {
    printf("\033[?25l");
}

void Canvas_showCursor() {
    printf("\033[?25h");
}

void Canvas_clearScreen() {
    system("clear");
}