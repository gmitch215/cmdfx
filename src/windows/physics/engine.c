#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <process.h>
#include <time.h>

#include "cmdfx/core/util.h"
#include "cmdfx/physics/motion.h"
#include "cmdfx/physics/engine.h"

int _physicsRunning = 0;

unsigned __stdcall Engine_applyMotion0(void* arg) {
    CmdFX_Sprite* sprite = (CmdFX_Sprite*) arg;
    if (sprite == 0) return 0;

    Engine_applyMotion(sprite);
    return 0;
}

unsigned __stdcall _physicsLoop(void* arg) {
    _physicsRunning = 1;

    int sleep = (int) ((1000.0 / CmdFX_getTickSpeed()) * 1000000);

    while (_physicsRunning) {
        CmdFX_Sprite** modified = Engine_tick();

        int i = 0;
        while (modified[i] != 0) {
            uintptr_t handle = _beginthreadex(NULL, 0, Engine_applyMotion0, modified[i], 0, NULL);
            if (handle == 0) {
                perror("Failed to start physics engine thread for an inside sprite.\n");
                exit(EXIT_FAILURE);
            }

            CloseHandle((HANDLE) handle);
            i++;
        }

        free(modified);
        sleepNanos(sleep);
    }

    return 0;
}

int Engine_start() {
    if (_physicsRunning) return -1;

    uintptr_t physicsEngineThread;
    physicsEngineThread = _beginthreadex(NULL, 0, _physicsLoop, NULL, 0, NULL);
    if (physicsEngineThread == 0) {
        perror("Failed to start physics engine loop.\n");
        exit(EXIT_FAILURE);
    }

    CloseHandle((HANDLE) physicsEngineThread);

    return 0;
}

int Engine_end() {
    if (!_physicsRunning) return -1;

    _physicsRunning = 0;

    return Engine_cleanup();
}