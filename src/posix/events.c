#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>

#include "cmdfx/canvas.h"
#include "cmdfx/events.h"
#include "cmdfx/util.h"
#include "cmdfx/screen.h"
#include "cmdfx/device.h"

// Core Events

int _prevWidth = 0;
int _prevHeight = 0;

void posix_checkResizeEvent(int sig) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        return;
    }

    if (ws.ws_col != _prevWidth || ws.ws_row != _prevHeight) {
        CmdFX_ResizeEvent resize = {_prevWidth, _prevHeight, ws.ws_col, ws.ws_row};
        CmdFX_Event event = {CMDFX_EVENT_RESIZE, currentTimeMillis(), &resize};
        dispatchCmdFXEvent(&event);

        _prevWidth = ws.ws_col;
        _prevHeight = ws.ws_row;
    }
}

int* _prevKeys = 0;

void posix_checkKeyEvent() {
    int* keys = Device_getKeyboardKeysPressed();
    if (_prevKeys == 0)
        _prevKeys = (int*) calloc(256, sizeof(int));

    for (int i = 0; i < 256; i++) {
        if (keys[i] != _prevKeys[i]) {
            CmdFX_KeyEvent keyEvent = {i, Device_fromKeyCode(i), keys[i]};
            CmdFX_Event event = {CMDFX_EVENT_KEY, currentTimeMillis(), &keyEvent};
            dispatchCmdFXEvent(&event);
            _prevKeys[i] = keys[i];
        }
    }

    free(keys);
}

int* _prevButtons = 0;
int _prevMouseX = -1;
int _prevMouseY = -1;

void posix_checkMouseEvent() {
    int* buttons = Device_getMouseButtonsPressed();
    if (_prevButtons == 0)
        _prevButtons = (int*) calloc(3, sizeof(int));

    int x, y;
    Screen_getMousePos(&x, &y);

    for (int i = 0; i < 3; i++) {
        if (buttons[i] != _prevButtons[i] || x != _prevMouseX || y != _prevMouseY) {
            CmdFX_MouseEvent mouseEvent = {i, buttons[i], _prevMouseX, x, _prevMouseY, y};
            CmdFX_Event event = {CMDFX_EVENT_MOUSE, currentTimeMillis(), &mouseEvent};
            dispatchCmdFXEvent(&event);

            _prevButtons[i] = buttons[i];
            _prevMouseX = x;
            _prevMouseY = y;
        }
    }

    free(buttons);
}

// Event Loop

void initSignalHandlers() {
    struct sigaction sa;
    sa.sa_handler = posix_checkResizeEvent;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGWINCH, &sa, NULL);
}

int _running = 0;

void* _eventLoop(void* arg) {
    _running = 1;

    while (_running) {
        posix_checkKeyEvent();
        posix_checkMouseEvent();

        usleep(EVENT_TICK * 1000);
    }

    return 0;
}

int beginCmdFXEventLoop() {
    if (_running) return 0;

    pthread_t eventLoopThread;
    if (pthread_create(&eventLoopThread, 0, _eventLoop, 0) != 0) {
        fprintf(stderr, "Failed to start event loop.\n");
        exit(1);
    }

    pthread_detach(eventLoopThread);

    initSignalHandlers();
    posix_checkResizeEvent(0);

    return 1;
}

int endCmdFXEventLoop() {
    if (!_running) return 0;

    // free up loose variables
    if (_prevKeys) free(_prevKeys);
    if (_prevButtons) free(_prevButtons);

    _running = 0;
    return 1;
}