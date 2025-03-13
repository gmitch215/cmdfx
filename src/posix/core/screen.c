#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "cmdfx/core/screen.h"

// Window API

static char windowTitle[256] = "Terminal";

const char* Window_getTitle() {
    return windowTitle;
}

void Window_setTitle(const char* title) {
    if (strlen(title) > sizeof(windowTitle) - 1) {
        fprintf(stderr, "Window title is too long.\n");
        return;
    }

    strncpy(windowTitle, title, sizeof(windowTitle) - 1);
    printf("\033]0;%s\007", title);
}

void Window_getSize(int* width, int* height) {
    if (width == 0) return;
    if (height == 0) return;

    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        *width = ws.ws_col;
        *height = ws.ws_row;
    } else {
        *width = *height = 0;
    }
}

void Window_setSize(int width, int height) {
    struct winsize w;
    w.ws_col = width;
    w.ws_row = height;
    
    if (ioctl(STDOUT_FILENO, TIOCSWINSZ, &w) == -1) {
        perror("ioctl");
    }
}

// Screen API

static struct termios original;
static int original_saved = 0;

static void ensure_saved_settings() {
    if (!original_saved) {
        tcgetattr(STDIN_FILENO, &original);
        original_saved = 1;
    }
}

int Screen_isEchoEnabled() {
    struct termios term;
    ensure_saved_settings();
    tcgetattr(STDIN_FILENO, &term);
    
    return (term.c_lflag & ECHO) ? 1 : 0;
}

int Screen_setEchoEnabled(int enabled) {
    struct termios term;
    ensure_saved_settings();
    tcgetattr(STDIN_FILENO, &term);
    
    if (enabled)
        term.c_lflag |= ECHO;
    else
        term.c_lflag &= ~ECHO;
    
    return (tcsetattr(STDIN_FILENO, TCSANOW, &term) == 0) ? 0 : -1;
}

int Screen_isLineBuffered() {
    struct termios term;
    ensure_saved_settings();
    tcgetattr(STDIN_FILENO, &term);

    return (term.c_lflag & ICANON) ? 1 : 0;
}

int Screen_setLineBuffered(int enabled) {
    struct termios term;
    ensure_saved_settings();
    tcgetattr(STDIN_FILENO, &term);
    
    if (enabled)
        term.c_lflag |= ICANON;
    else
        term.c_lflag &= ~ICANON;
    
    return (tcsetattr(STDIN_FILENO, TCSANOW, &term) == 0) ? 0 : -1;
}

int Screen_isInTerminal() {
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
}

void launchInTerminal() {
    char exe_path[1024];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("readlink");
        return 1;
    }
    exe_path[len] = '\0';

    const char *terminal = "x-terminal-emulator"; // Debian-based
    if (access("/usr/bin/gnome-terminal", X_OK) == 0) {
        terminal = "gnome-terminal";
    } else if (access("/usr/bin/konsole", X_OK) == 0) {
        terminal = "konsole";
    } else if (access("/usr/bin/xterm", X_OK) == 0) {
        terminal = "xterm";
    }

    char command[2048];
    snprintf(command, sizeof(command), "%s -e \"%s\"", terminal, exe_path);

    system(command);
}

int Screen_ensureInTerminal() {
    if (!Screen_isInTerminal()) {
        fprintf(stderr, "This program must be run in a terminal.\n");
        launchInTerminal();
        exit(1);
    }
    return 0;
}