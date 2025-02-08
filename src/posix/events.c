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

int _prevKey = 0;

void posix_checkKeyEvent() {
    int key = Device_getKeyboardKeyPressed();
    if (key == -1) return;

    if (key != _prevKey) {
        CmdFX_KeyEvent keyEvent = {_prevKey, key};
        CmdFX_Event event = {CMDFX_EVENT_KEY, currentTimeMillis(), &keyEvent};
        dispatchCmdFXEvent(&event);

        _prevKey = key;
    }
}

int _prevButton = 0;
int _prevMouseX = -1;
int _prevMouseY = -1;

void posix_checkMouseEvent() {
    int button = Device_getMouseButtonPressed();
    if (button == -1) return;

    int x, y;
    Screen_getMousePos(&x, &y);

    if (button != _prevButton || x != _prevMouseX || y != _prevMouseY) {
        CmdFX_MouseEvent mouseEvent = {_prevButton, button, _prevMouseX, x, _prevMouseY, y};
        CmdFX_Event event = {CMDFX_EVENT_MOUSE, currentTimeMillis(), &mouseEvent};
        dispatchCmdFXEvent(&event);

        _prevButton = button;
        _prevMouseX = x;
        _prevMouseY = y;
    }
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

    _running = 0;
    return 1;
}