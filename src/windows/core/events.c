#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <process.h>
#include <time.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/events.h"
#include "cmdfx/core/util.h"
#include "cmdfx/core/screen.h"
#include "cmdfx/core/device.h"

// Core Events

int _prevWidth = 0;
int _prevHeight = 0;

void win_checkResizeEvent() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        if (width != _prevWidth || height != _prevHeight) {
            CmdFX_ResizeEvent resize = {_prevWidth, _prevHeight, width, height};
            CmdFX_Event event = {CMDFX_EVENT_RESIZE, currentTimeMillis(), &resize};
            dispatchCmdFXEvent(&event);

            _prevWidth = width;
            _prevHeight = height;
        }
    }
}

int* _prevKeys = 0;

void win_checkKeyEvent() {
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

void win_checkMouseEvent() {
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

int _running = 0;

unsigned __stdcall _eventLoop(void* arg) {
    _running = 1;

    while (_running) {
        win_checkResizeEvent();
        win_checkKeyEvent();
        win_checkMouseEvent();

        Sleep(EVENT_TICK);
    }

    return 0;
}

int beginCmdFXEventLoop() {
    if (_running) return 0;

    uintptr_t eventLoopThread;
    eventLoopThread = _beginthreadex(NULL, 0, _eventLoop, NULL, 0, NULL);
    if (eventLoopThread == 0) {
        perror("Failed to start event loop.\n");
        exit(EXIT_FAILURE);
    }

    CloseHandle((HANDLE) eventLoopThread);
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