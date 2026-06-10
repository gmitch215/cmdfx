#include <process.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

static atomic_int _physicsRunning = 0;
static HANDLE _physicsThread = NULL;

unsigned __stdcall _physicsLoop(void* arg) {
    (void) arg;

    int sleep = (int) ((1000.0 / CmdFX_getTickSpeed()) * 1000000);

    while (atomic_load(&_physicsRunning)) {
        CmdFX_Sprite** modified = Engine_tick();

        if (modified != 0) {
            // apply motion synchronously for a deterministic integration order
            for (int i = 0; modified[i] != 0; i++)
                Engine_applyMotion(modified[i]);

            free(modified);
        }

        fflush(stdout);
        sleepNanos(sleep);
    }

    return 0;
}

int Engine_start() {
    if (atomic_load(&_physicsRunning)) return -1;

    if (!CmdFX_isThreadSafeEnabled()) CmdFX_initThreadSafe();

    atomic_store(&_physicsRunning, 1);
    uintptr_t thread = _beginthreadex(NULL, 0, _physicsLoop, NULL, 0, NULL);
    if (thread == 0) {
        atomic_store(&_physicsRunning, 0);
        fprintf(stderr, "Failed to start physics engine loop.\n");
        return -1;
    }

    _physicsThread = (HANDLE) thread;
    return 0;
}

int Engine_end() {
    if (!atomic_load(&_physicsRunning)) return -1;
    atomic_store(&_physicsRunning, 0);

    // join the loop before freeing engine state to avoid a use after free
    if (_physicsThread != NULL) {
        WaitForSingleObject(_physicsThread, INFINITE);
        CloseHandle(_physicsThread);
        _physicsThread = NULL;
    }

    if (CmdFX_isThreadSafeEnabled()) CmdFX_destroyThreadSafe();

    return Engine_cleanup();
}