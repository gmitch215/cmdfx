#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/util.h"
#include "cmdfx/physics/force.h"

// Impulse Functions

typedef struct _PosixForcePayload {
    CmdFX_Sprite* sprite;
    CmdFX_Vector* vector;
    time_t sec;
    time_t nsec;
} _PosixForcePayload;

void* _addSpriteForce(void* arg) {
    _PosixForcePayload* payload = (_PosixForcePayload*) arg;

    struct timespec ts;
    ts.tv_sec = payload->sec;
    ts.tv_nsec = payload->nsec;

    Sprite_addForce(payload->sprite, payload->vector);
    nanosleep(&ts, NULL);
    Sprite_removeForce(payload->sprite, payload->vector);

    free(payload);
    return 0;
}

int Sprite_addForceFor(CmdFX_Sprite* sprite, CmdFX_Vector* vector, int duration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (vector == 0) return -1;
    if (duration <= 0) return -1;

    _PosixForcePayload* payload = malloc(sizeof(_PosixForcePayload));
    if (payload == 0) return -1;

    payload->sprite = sprite;
    payload->vector = vector;
    payload->sec = duration / 1000;
    payload->nsec = (duration % 1000) * 1000000;

    pthread_t forceThread;
    if (pthread_create(&forceThread, 0, _addSpriteForce, payload) != 0) {
        fprintf(stderr, "Failed to start new thread to launch force payload.\n");
        exit(1);
    }
    pthread_detach(forceThread);

    return 0;
}