#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "cmdfx/core/util.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/util.h"
#include "cmdfx/physics/force.h"

// Impulse Functions

typedef struct _PosixForcePayload {
    CmdFX_Sprite* sprite;
    CmdFX_Vector* vector;
    unsigned long msec;
} _PosixForcePayload;

void* _addSpriteForce(void* arg) {
    _PosixForcePayload* payload = (_PosixForcePayload*) arg;

    Sprite_addForce(payload->sprite, payload->vector);
    sleepMillis(payload->msec);
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
    payload->msec = (unsigned long) duration;

    pthread_t forceThread;
    if (pthread_create(&forceThread, 0, _addSpriteForce, payload) != 0) {
        fprintf(stderr, "Failed to start new thread to launch force payload.\n");
        free(payload);
        exit(1);
    }
    pthread_detach(forceThread);

    return 0;
}