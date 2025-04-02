#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>

#include "cmdfx/core/util.h"
#include "cmdfx/physics/motion.h"
#include "cmdfx/physics/engine.h"

int _physicsRunning = 0;

void* Engine_applyMotion0(void* arg) {
    CmdFX_Sprite* sprite = (CmdFX_Sprite*) arg;
    if (sprite == 0) return 0;

    Engine_applyMotion(sprite);
    return 0;
}

void* _physicsLoop(void* data) {
    _physicsRunning = 1;

    int sleep = (int) ((1000.0 / CmdFX_getTickSpeed()) * 1000000);

    while (_physicsRunning) {
        CmdFX_Sprite** modified = Engine_tick();

        int i = 0;
        while (modified[i] != 0) {
            pthread_t thread;
            if (pthread_create(&thread, NULL, Engine_applyMotion0, modified[i]) != 0) {
                fprintf(stderr, "Failed to start physics engine thread for an inside sprite.\n");
                exit(EXIT_FAILURE);
            }

            pthread_detach(thread);
            i++;
        }
        
        free(modified);
        sleepNanos(sleep);
    }

    return 0;
}

int Engine_start() {
    if (_physicsRunning) return -1;

    pthread_t physicsEngineThread;
    if (pthread_create(&physicsEngineThread, 0, _physicsLoop, 0) != 0) {
        fprintf(stderr, "Failed to start physics engine thread.\n");
        exit(1);
    }

    pthread_detach(physicsEngineThread);

    return 0;
}

int Engine_end() {
    if (!_physicsRunning) return -1;

    _physicsRunning = 0;

    return Engine_cleanup();
}