#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "cmdfx/screen.h"

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
        exit(EXIT_FAILURE);
    }
}