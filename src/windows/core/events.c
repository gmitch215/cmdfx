#include <process.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/events.h"
#include "cmdfx/core/util.h"
#include "cmdfx/ui/button.h"
#include "cmdfx/ui/switch.h"
#include "common/core/curses_backend.h"

// Core Events

int _prevWidth = -1;
int _prevHeight = -1;

int Canvas_getWidth() {
    int width, height;
    CmdFX_curses_getSize(&width, &height);
    return width;
}

int Canvas_getHeight() {
    int width, height;
    CmdFX_curses_getSize(&width, &height);
    return height;
}

// Event Dispatch

static void _dispatchKey(const CmdFX_CursesEvent* e) {
    CmdFX_KeyEvent keyEvent = {e->keyCode, e->keyChar, e->mousePressed};
    CmdFX_Event event = {CMDFX_EVENT_KEY, currentTimeMillis(), &keyEvent};
    dispatchCmdFXEvent(&event);
}

static void _dispatchResize(const CmdFX_CursesEvent* e) {
    CmdFX_ResizeEvent resize = {_prevWidth, _prevHeight, e->width, e->height};
    CmdFX_Event event = {CMDFX_EVENT_RESIZE, currentTimeMillis(), &resize};
    dispatchCmdFXEvent(&event);

    _prevWidth = e->width;
    _prevHeight = e->height;
}

static int _prevMouseX = -1;
static int _prevMouseY = -1;

static void _dispatchMouse(const CmdFX_CursesEvent* e) {
    unsigned long long time = currentTimeMillis();
    CmdFX_MouseEvent mouseEvent = {e->mouseButton, e->mousePressed, _prevMouseX,
                                   e->mouseX,      _prevMouseY,     e->mouseY};
    CmdFX_Event event = {CMDFX_EVENT_MOUSE, time, &mouseEvent};
    dispatchCmdFXEvent(&event);

    // button events
    CmdFX_Button** allButtons = Canvas_getAllButtonsAt(e->mouseX, e->mouseY);
    if (allButtons != 0) {
        int j = 0;
        while (allButtons[j] != 0) {
            CmdFX_Button* button = allButtons[j];
            CmdFX_ButtonCallback callback = *button->callback;
            callback(button, &mouseEvent, time);

            CmdFX_ButtonEvent buttonEvent = {&mouseEvent, button};
            CmdFX_Event buttonEventStruct = {
                CMDFX_EVENT_BUTTON_CLICK, time, &buttonEvent
            };
            dispatchCmdFXEvent(&buttonEventStruct);

            switch (button->type) {
                case CMDFX_BUTTON_TYPE_SWITCH:
                    Switch_toggleState(button);
                    break;
                default: break;
            }

            j++;
        }
    }
    free(allButtons);

    _prevMouseX = e->mouseX;
    _prevMouseY = e->mouseY;
}

// Event Loop

int _eventsRunning = 0;

unsigned __stdcall _eventLoop(void* arg) {
    (void) arg;
    _eventsRunning = 1;

    while (_eventsRunning) {
        // drain every event curses has queued, then yield
        CmdFX_CursesEvent e;
        while (CmdFX_curses_poll(&e)) {
            switch (e.type) {
                case CMDFX_CURSES_EVENT_KEY: _dispatchKey(&e); break;
                case CMDFX_CURSES_EVENT_MOUSE: _dispatchMouse(&e); break;
                case CMDFX_CURSES_EVENT_RESIZE: _dispatchResize(&e); break;
                default: break;
            }
        }

        sleepMillis(EVENT_TICK);
    }

    return 0;
}

int beginCmdFXEventLoop() {
    if (_eventsRunning) return 0;

    CmdFX_curses_ensure();
    CmdFX_curses_getSize(&_prevWidth, &_prevHeight);

    uintptr_t eventLoopThread =
        _beginthreadex(NULL, 0, _eventLoop, NULL, 0, NULL);
    if (eventLoopThread == 0) {
        fprintf(stderr, "Failed to start event loop.\n");
        return 0;
    }

    CloseHandle((HANDLE) eventLoopThread);
    return 1;
}

int endCmdFXEventLoop() {
    if (!_eventsRunning) return 0;

    _eventsRunning = 0;
    return 1;
}
