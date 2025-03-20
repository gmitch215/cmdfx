#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <process.h>

#include "cmdfx/core/scenes.h"
#include "cmdfx/core/util.h"

int _scenesRunning = 0;

unsigned __stdcall _sceneLoop(void* arg) {
    _scenesRunning = 1;

    int sleep = (int) ((1000.0 / CmdFX_getTickSpeed()));

    while (_scenesRunning) {
        tickCmdFXSceneEngine();
        Sleep(sleep);
    }

    return 0;
}

int beginCmdFXSceneEngine() {
    if (_scenesRunning) return 0;

    uintptr_t sceneEngineThread;
    sceneEngineThread = _beginthreadex(NULL, 0, _sceneLoop, NULL, 0, NULL);
    if (sceneEngineThread == 0) {
        perror("Failed to start scene engine thread.\n");
        exit(EXIT_FAILURE);
    }

    CloseHandle((HANDLE) sceneEngineThread);
    return 1;
}

int endCmdFXSceneEngine() {
    if (!_scenesRunning) return 0;

    _scenesRunning = 0;
    return 1;
}