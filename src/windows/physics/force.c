#include <process.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/util.h"

// Impulse Functions

typedef struct _WinForcePayload {
    CmdFX_Sprite* sprite;
    CmdFX_Vector* vector;
    int duration;
} _WinForcePayload;

unsigned __stdcall _addSpriteForce(void* arg) {
    _WinForcePayload* payload = (_WinForcePayload*) arg;

    Sprite_addForce(payload->sprite, payload->vector);
    Sleep(payload->duration);
    Sprite_removeForce(payload->sprite, payload->vector);

    free(payload);
    return 0;
}

int Sprite_addForceFor(
    CmdFX_Sprite* sprite, CmdFX_Vector* vector, int duration
) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (vector == 0) return -1;
    if (duration <= 0) return -1;

    _WinForcePayload* payload = malloc(sizeof(_WinForcePayload));
    if (payload == 0) return -1;

    payload->sprite = sprite;
    payload->vector = vector;
    payload->duration = duration;

    uintptr_t forceThread;
    forceThread = _beginthreadex(NULL, 0, _addSpriteForce, payload, 0, NULL);
    if (forceThread == 0) {
        perror("Failed to start new thread to launch force payload.\n");
        free(payload);
        exit(EXIT_FAILURE);
    }
    CloseHandle((HANDLE) forceThread);

    return 0;
}