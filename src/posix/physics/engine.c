#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

static atomic_int _physicsRunning = 0;
static pthread_t _physicsThread;
static int _physicsThreadValid = 0;

void* _physicsLoop(void* data) {
    (void) data;

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
    if (pthread_create(&_physicsThread, 0, _physicsLoop, 0) != 0) {
        atomic_store(&_physicsRunning, 0);
        fprintf(stderr, "Failed to start physics engine thread.\n");
        return -1;
    }

    _physicsThreadValid = 1;
    return 0;
}

int Engine_end() {
    if (!atomic_load(&_physicsRunning)) return -1;
    atomic_store(&_physicsRunning, 0);

    // join the loop before freeing engine state to avoid a use after free
    if (_physicsThreadValid) {
        pthread_join(_physicsThread, 0);
        _physicsThreadValid = 0;
    }

    if (CmdFX_isThreadSafeEnabled()) CmdFX_destroyThreadSafe();

    return Engine_cleanup();
}