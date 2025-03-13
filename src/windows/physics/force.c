#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <process.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/util.h"
#include "cmdfx/physics/force.h"

// Impulse Functions

typedef struct _WinImpulsePayload {
    CmdFX_Sprite* sprite;
    CmdFX_Vector* vector;
    int duration;
} _WinImpulsePayload;

unsigned __stdcall _addSpriteImpulse(void* arg) {
    _WinImpulsePayload* payload = (_WinImpulsePayload*) arg;

    Sprite_addForce(payload->sprite, payload->vector);
    Sleep(payload->duration);
    Sprite_removeForce(payload->sprite, payload->vector);

    free(payload);
    return 0;
}

int Sprite_addImpulse(CmdFX_Sprite* sprite, CmdFX_Vector* vector, int duration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (vector == 0) return -1;
    if (duration <= 0) return -1;

    _WinImpulsePayload* payload = malloc(sizeof(_WinImpulsePayload));
    if (payload == 0) return -1;

    payload->sprite = sprite;
    payload->vector = vector;
    payload->duration = duration;

    uintptr_t impulseThread;
    impulseThread = _beginthreadex(NULL, 0, _addSpriteImpulse, payload, 0, NULL);
    if (impulseThread == 0) {
        perror("Failed to start new thread to launch impulse.\n");
        exit(EXIT_FAILURE);
    }
    CloseHandle((HANDLE) impulseThread);

    return 0;
}