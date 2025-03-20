#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "cmdfx/core/scenes.h"
#include "cmdfx/core/util.h"

int _scenesRunning = 0;

void* _sceneLoop(void* arg) {
    _scenesRunning = 1;

    int sleep = (int) ((1000.0 / CmdFX_getTickSpeed()) * 1000);

    while (_scenesRunning) {
        tickCmdFXSceneEngine();
        usleep(sleep);
    }

    return 0;
}

int beginCmdFXSceneEngine() {
    if (_scenesRunning) return 0;

    pthread_t sceneEngineThread;
    if (pthread_create(&sceneEngineThread, 0, _sceneLoop, 0) != 0) {
        fprintf(stderr, "Failed to start scene engine loop.\n");
        exit(1);
    }

    pthread_detach(sceneEngineThread);

    return 1;
}

int endCmdFXSceneEngine() {
    if (!_scenesRunning) return 0;

    _scenesRunning = 0;
    return 1;
}