#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>

#include "cmdfx/canvas.h"

int _width = 0;
int _height = 0;
int _color = COLOR_WHITE;

int Canvas_getWidth() {
    if (_width != 0) return &_width;
    
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        _width = ws.ws_col;
    }
    return _width;
}

int Canvas_getHeight() {
    if (_height != 0) return &_height;
    
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        _height = ws.ws_col;
    }
    return _height;
}

int Canvas_getColor() {
    return _color;
}

void Canvas_setColor(int color) {
    _color = color;
    printf("\033[38;5;%dm", color);
}

void Canvas_setCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void Canvas_clearScreen() {
    system("clear");
}