#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmdfx/core/screen.h"
#include "common/core/curses_backend.h"

// Window API

static char windowTitle[256] = "Terminal";

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

    // curses has no title primitive; emit the OSC sequence out of band
    if (isatty(STDOUT_FILENO)) {
        printf("\033]0;%s\007", title);
        fflush(stdout);
    }
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
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
}

void launchInTerminal() {
    char exe_path[1024];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("readlink");
        return;
    }
    exe_path[len] = '\0';

    const char* terminal = "x-terminal-emulator"; // Debian-based

    if (access("/usr/bin/gnome-terminal", X_OK) == 0)
        terminal = "gnome-terminal";
    else if (access("/usr/bin/konsole", X_OK) == 0)
        terminal = "konsole";
    else if (access("/usr/bin/xterm", X_OK) == 0)
        terminal = "xterm";

    char command[2048];
    snprintf(command, sizeof(command), "%s -e \"%s\"", terminal, exe_path);

    system(command);
}

void Screen_ensureInTerminal() {
    if (!Screen_isInTerminal()) {
        fprintf(stderr, "This program must be run in a terminal.\n");
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
    // curses has no current-mouse-position query; mouse position now arrives
    // through CMDFX_EVENT_MOUSE events instead
    if (x) *x = 0;
    if (y) *y = 0;
}
