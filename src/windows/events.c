#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <process.h>
#include <time.h>

#include "cmdfx/canvas.h"
#include "cmdfx/events.h"

int _currentTimeMillis() {
    return (int) (clock() / (CLOCKS_PER_SEC / 1000));
}

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
            CmdFX_Event event = {CMDFX_EVENT_RESIZE, _currentTimeMillis(), &resize};
            dispatchCmdFXEvent(&event);

            _prevWidth = width;
            _prevHeight = height;
        }
    }
}

// Event Loop

int _running = 0;

unsigned __stdcall _eventLoop(void* arg) {
    _running = 1;

    while (_running) {
        win_checkResizeEvent();
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