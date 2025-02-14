#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmdfx/physics/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/force.h"

CmdFX_Vector*** _forces = 0;

CmdFX_Vector** Sprite_getAllForces(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;

    return _forces[sprite->id];
}

CmdFX_Vector* Sprite_getNetForce(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;

    CmdFX_Vector** forces = _forces[sprite->id];
    if (forces == 0) return 0;

    CmdFX_Vector* netForce = Vector_create(0, 0);
    if (netForce == 0) return 0;

    for (int i = 0; forces[i] != 0; i++) {
        CmdFX_Vector* force = forces[i];
        netForce = Vector_add(netForce, force);
    }

    return netForce;
}

int Sprite_addForce(CmdFX_Sprite* sprite, CmdFX_Vector* vector) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    CmdFX_Sprite** sprites = Canvas_getDrawnSprites();
    int count = 0;
    while (sprites[count] != 0) count++;

    if (_forces == 0) {
        _forces = calloc(count + 1, sizeof(CmdFX_Vector**));
        if (_forces == 0) return -1;

        _forces[count] = 0;
    } else {
        int forcesCount = 0;
        while (_forces[forcesCount] != 0) forcesCount++;

        if (forcesCount != count) {
            CmdFX_Vector*** temp = realloc(_forces, sizeof(CmdFX_Vector**) * (count + 1));
            if (temp == 0) return -1;

            _forces = temp;
            _forces[count] = 0;
        }
    }

    CmdFX_Vector** forces = _forces[sprite->id];
    if (forces == 0) {
        forces = malloc(sizeof(CmdFX_Vector*) * 2);
        if (forces == 0) return -1;

        forces[0] = vector;
        forces[1] = 0;

        _forces[sprite->id] = forces;
        return 0;
    }

    int i = 0;
    while (forces[i] != 0) i++;

    CmdFX_Vector** temp = realloc(forces, sizeof(CmdFX_Vector*) * (i + 2));
    if (temp == 0) return -1;

    forces = temp;
    forces[i] = vector;
    forces[i + 1] = 0;
    _forces[sprite->id] = forces;

    return 0;
}

int Sprite_removeForce(CmdFX_Sprite* sprite, CmdFX_Vector* vector) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    CmdFX_Vector** forces = _forces[sprite->id];
    if (forces == 0) return -1;

    int i = 0;
    while (forces[i] != 0) {
        if (forces[i] == vector) {
            free(forces[i]);

            for (int j = i; forces[j] != 0; j++)
                forces[j] = forces[j + 1];

            int count = 0;
            while (forces[count] != 0) count++;

            CmdFX_Vector** temp = realloc(forces, sizeof(CmdFX_Vector*) * count);
            if (temp == 0) return -1;

            forces = temp;
            _forces[sprite->id] = forces;

            return 0;
        }

        i++;
    }

    return -1;
}

int Sprite_removeAllForces(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    CmdFX_Vector** forces = _forces[sprite->id];
    if (forces == 0) return -1;

    for (int i = 0; forces[i] != 0; i++)
        free(forces[i]);

    free(forces);
    _forces[sprite->id] = 0;

    return 0;
}