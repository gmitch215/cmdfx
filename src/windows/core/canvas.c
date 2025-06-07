#include <stdlib.h>
#include <windows.h>

#include "cmdfx/core/util.h"
#include "cmdfx/core/canvas.h"

#define _CANVAS_MUTEX 7

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
    if (x < 0) return;
    if (y < 0) return;

    COORD coord = {x - 1, y - 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Canvas_hideCursor() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_showCursor() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

int Canvas_isCursorVisible() {
    CONSOLE_CURSOR_INFO cursor_info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleCursorInfo(hConsole, &cursor_info)) {
        return cursor_info.bVisible;
    } else {
        perror("Error getting cursor info");
        return 0;
    }
}

void Canvas_clearScreen() {
    system("cls");
}