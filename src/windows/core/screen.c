#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "cmdfx/core/screen.h"
#include "common/core/curses_backend.h"

// Window API

static char windowTitle[256] = "Command Prompt";

const char* Window_getTitle() {
    return windowTitle;
}

void Window_setTitle(const char* title) {
    if (title == 0) return;
    if (strlen(title) > sizeof(windowTitle) - 1) {
        fprintf(stderr, "Window title is too long.\n");
        return;
    }

    strncpy(windowTitle, title, sizeof(windowTitle) - 1);
    windowTitle[sizeof(windowTitle) - 1] = '\0';

    // SetConsoleTitleA is native and harmless alongside the curses screen
    SetConsoleTitleA(title);
}

void Window_getSize(int* width, int* height) {
    CmdFX_curses_getSize(width, height);
}

void Window_setSize(int width, int height) {
    // curses does not resize the host terminal; the user controls window size
    (void) width;
    (void) height;
}

// Screen API

int Screen_isEchoEnabled() {
    return CmdFX_curses_getEcho();
}

int Screen_setEchoEnabled(int enabled) {
    CmdFX_curses_setEcho(enabled);
    return 0;
}

int Screen_isLineBuffered() {
    return CmdFX_curses_getLineBuffered();
}

int Screen_setLineBuffered(int enabled) {
    CmdFX_curses_setLineBuffered(enabled);
    return 0;
}

int Screen_isInTerminal() {
    return GetConsoleWindow() != 0;
}

void launchInTerminal() {
    char command[MAX_PATH];
    GetModuleFileName(NULL, command, MAX_PATH);

    char cmd[MAX_PATH + 20];
    snprintf(cmd, sizeof(cmd), "cmd /c start cmd /k \"%s\"", command);
    system(cmd);
}

void Screen_ensureInTerminal() {
    if (!Screen_isInTerminal()) {
        fprintf(stderr, "Program must be run in a terminal.\n");
        launchInTerminal();
        exit(1);
    }
}

// repurposed to terminal cells; the old physical-pixel sizing relied on
// platform window frameworks that the curses migration removed

void Screen_getSize(int* width, int* height) {
    CmdFX_curses_getSize(width, height);
}

void Screen_getMousePos(int* x, int* y) {
    if (x) *x = 0;
    if (y) *y = 0;
}
