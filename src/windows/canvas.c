#include <stdlib.h>
#include <windows.h>

#include "cmdfx/canvas.h"

int _width = 0;
int _height = 0;
int _color = COLOR_WHITE;

int Canvas_getWidth() {
    if (_width != 0) return _width;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        _width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    
    return _width;
}

int Canvas_getHeight() {
    if (_height != 0) return _height;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        _height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    return _height;
}

int Canvas_getCursorX() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.dwCursorPosition.X;
    }

    return -1;
}

int Canvas_getCursorY() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.dwCursorPosition.Y;
    }

    return -1;
}

void Canvas_setCursor(int x, int y) {
    COORD coord = {x - 1, y - 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int Canvas_getColor() {
    return _color;
}

void Canvas_setColor(int color) {
    _color = color;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Canvas_clearScreen() {
    system("cls");
}