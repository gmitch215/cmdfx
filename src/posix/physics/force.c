#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/util.h"
#include "cmdfx/physics/force.h"

// Impulse Functions

typedef struct _PosixImpulsePayload {
    CmdFX_Sprite* sprite;
    CmdFX_Vector* vector;
    int duration;
} _PosixImpulsePayload;

void* _addSpriteImpulse(void* arg) {
    _PosixImpulsePayload* payload = (_PosixImpulsePayload*) arg;

    Sprite_addForce(payload->sprite, payload->vector);
    usleep(payload->duration);
    Sprite_removeForce(payload->sprite, payload->vector);

    free(payload);
    return 0;
}

int Sprite_addImpulse(CmdFX_Sprite* sprite, CmdFX_Vector* vector, int duration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (vector == 0) return -1;
    if (duration <= 0) return -1;

    _PosixImpulsePayload* payload = malloc(sizeof(_PosixImpulsePayload));
    if (payload == 0) return -1;

    payload->sprite = sprite;
    payload->vector = vector;
    payload->duration = duration * 1000;

    pthread_t impulseThread;
    if (pthread_create(&impulseThread, 0, _addSpriteImpulse, payload) != 0) {
        fprintf(stderr, "Failed to start new thread to launch impulse.\n");
        exit(1);
    }
    pthread_detach(impulseThread);

    return 0;
}