#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

int _physicsRunning = 0;

void* Engine_applyMotion0(void* arg) {
    CmdFX_Sprite* sprite = (CmdFX_Sprite*) arg;
    if (sprite == 0) return 0;

    Engine_applyMotion(sprite);
    fflush(stdout);
    return 0;
}

void* _physicsLoop(void* data) {
    _physicsRunning = 1;

    int sleep = (int) ((1000.0 / CmdFX_getTickSpeed()) * 1000000);

    while (_physicsRunning) {
        CmdFX_Sprite** modified = Engine_tick();

        if (modified != 0) {
            int i = 0;
            while (modified[i] != 0) {
                pthread_t thread;
                if (pthread_create(
                        &thread, NULL, Engine_applyMotion0, modified[i]
                    ) != 0) {
                    fprintf(
                        stderr,
                        "Failed to start physics engine thread for sprite "
                        "ID %u.\n",
                        modified[i]->id
                    );
                    // Continue processing other sprites instead of terminating
                    i++;
                    continue;
                }

                pthread_detach(thread);
                i++;
            }
        }

        free(modified);
        sleepNanos(sleep);
    }

    return 0;
}

int Engine_start() {
    if (_physicsRunning) return -1;

    if (!CmdFX_isThreadSafeEnabled()) CmdFX_initThreadSafe();

    pthread_t physicsEngineThread;
    if (pthread_create(&physicsEngineThread, 0, _physicsLoop, 0) != 0) {
        fprintf(stderr, "Failed to start physics engine thread.\n");
        return -1;
    }

    pthread_detach(physicsEngineThread);

    return 0;
}

int Engine_end() {
    if (!_physicsRunning) return -1;
    _physicsRunning = 0;

    if (CmdFX_isThreadSafeEnabled()) CmdFX_destroyThreadSafe();

    return Engine_cleanup();
}