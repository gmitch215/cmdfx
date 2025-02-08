#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <process.h>
#include <time.h>

#include "cmdfx/canvas.h"
#include "cmdfx/events.h"
#include "cmdfx/util.h"
#include "cmdfx/screen.h"
#include "cmdfx/device.h"

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

int _prevKey = 0;

void win_checkKeyEvent() {
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

void win_checkMouseEvent() {
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

    _running = 0;
    return 1;
}