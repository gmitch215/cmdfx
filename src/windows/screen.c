#include <stdio.h>
#include <windows.h>

#include "cmdfx/screen.h"

// Window API

static char windowTitle[256] = "Terminal";

const char* Window_getTitle() {
    HWND console = GetConsoleWindow();
    if (console) {
        GetWindowText(console, windowTitle, sizeof(windowTitle));
    }
    return windowTitle;
}

void Window_setTitle(const char* title) {
    if (strlen(title) > sizeof(windowTitle) - 1) {
        fprintf(stderr, "Window title is too long.\n");
        return;
    }

    SetConsoleTitle(title);
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

int Screen_getRGB(int x, int y) {
    HDC hdc = GetDC(NULL);  // Get the entire screen device context
    if (!hdc) {
        fprintf(stderr, "Failed to get device context.\n");
        return -1;
    }

    COLORREF color = GetPixel(hdc, x, y);
    ReleaseDC(NULL, hdc);

    if (color == CLR_INVALID) {
        fprintf(stderr, "Failed to get pixel color.\n");
        return -1;
    }

    int red = GetRValue(color);
    int green = GetGValue(color);
    int blue = GetBValue(color);

    return (red << 16) | (green << 8) | blue;
}