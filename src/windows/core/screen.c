#include <stdio.h>
#include <windows.h>

#include "cmdfx/core/screen.h"

// Window API

static char windowTitle[256] = "Command Prompt";

const char* Window_getTitle() {
    HWND console = GetConsoleWindow();
    if (console) {
        GetConsoleTitleA(windowTitle, sizeof(windowTitle));
    }

    return windowTitle;
}

void Window_setTitle(const char* title) {
    if (strlen(title) > sizeof(windowTitle) - 1) {
        fprintf(stderr, "Window title is too long.\n");
        return;
    }

    SetConsoleTitleA(title);
    strncpy(windowTitle, title, sizeof(windowTitle) - 1);
}

void Window_getSize(int* width, int* height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        *width = *height = 0;
    }
}

void Window_setSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, width - 1, height - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

// Screen API

void Screen_getSize(int* width, int* height) {
    if (width == 0) return;
    if (height == 0) return;

    *width = GetSystemMetrics(SM_CXSCREEN);
    *height = GetSystemMetrics(SM_CYSCREEN);
}

void Screen_getMousePos(int* x, int* y) {
    if (x == 0) return;
    if (y == 0) return;

    POINT p;
    if (!GetCursorPos(&p)) return;

    *x = p.x;
    *y = p.y;
}

static DWORD original;
static int original_saved = 0;

static void ensure_saved_settings() {
    if (!original_saved) {
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(hInput, &original);
        original_saved = 1;
    }
}

int Screen_isEchoEnabled() {
    DWORD mode;
    ensure_saved_settings();
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);

    return (mode & ENABLE_ECHO_INPUT) ? 1 : 0;
}

int Screen_setEchoEnabled(int enabled) {
    DWORD mode;
    ensure_saved_settings();
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hInput, &mode);

    if (enabled)
        mode |= ENABLE_ECHO_INPUT;
    else
        mode &= ~ENABLE_ECHO_INPUT;

    return (SetConsoleMode(hInput, mode) != 0) ? 0 : -1;
}

int Screen_isLineBuffered() {
    DWORD mode;
    ensure_saved_settings();
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);

    return (mode & ENABLE_LINE_INPUT) ? 1 : 0;
}

int Screen_setLineBuffered(int enabled) {
    DWORD mode;
    ensure_saved_settings();
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hInput, &mode);

    if (enabled)
        mode |= ENABLE_LINE_INPUT;
    else
        mode &= ~ENABLE_LINE_INPUT;
    
    return (SetConsoleMode(hInput, mode) != 0) ? 0 : -1;
}