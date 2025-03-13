#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmdfx/physics/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/force.h"

CmdFX_Vector*** _forces = 0;

CmdFX_Vector** Sprite_getAllForces(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->uid == 0) return 0;
    if (_forces == 0) return 0;

    int id = sprite->uid - 1;
    return _forces[id];
}

CmdFX_Vector* Sprite_getNetForce(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->uid == 0) return 0;
    if (_forces == 0) return 0;

    int id = sprite->uid - 1;
    CmdFX_Vector** forces = _forces[id];
    if (forces == 0) return 0;

    CmdFX_Vector* netForce = Vector_create(0, 0);
    if (netForce == 0) return 0;

    for (int i = 0; forces[i] != 0; i++) {
        CmdFX_Vector* force = forces[i];

        CmdFX_Vector* newForce = Vector_add(netForce, force);
        free(netForce);
        netForce = newForce;
    }

    return netForce;
}

int Sprite_addForce(CmdFX_Sprite* sprite, CmdFX_Vector* vector) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;

    int count = Canvas_getDrawnSpritesCount();
    int id = sprite->uid - 1;

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
            for (int i = forcesCount; i < count + 1; i++) temp[i] = 0;

            _forces = temp;
            _forces[count] = 0;
        }
    }

    CmdFX_Vector** forces = _forces[id];
    if (forces == 0) {
        forces = calloc(2, sizeof(CmdFX_Vector*));
        if (forces == 0) return -1;

        forces[0] = vector;
        forces[1] = 0;

        _forces[id] = forces;
        return 0;
    }

    int i = 0;
    while (forces[i] != 0) i++;

    CmdFX_Vector** temp = realloc(forces, sizeof(CmdFX_Vector*) * (i + 2));
    if (temp == 0) return -1;

    forces = temp;
    forces[i] = vector;
    forces[i + 1] = 0;
    _forces[id] = forces;

    return 0;
}

int Sprite_removeForce(CmdFX_Sprite* sprite, CmdFX_Vector* vector) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;

    int id = sprite->uid - 1;
    CmdFX_Vector** forces = _forces[id];
    if (forces == 0) return -1;

    int i = 0;
    while (forces[i] != 0) {
        if (forces[i] == vector) {
            forces[i] = 0;

            for (int j = i; forces[j] != 0; j++)
                forces[j] = forces[j + 1];

            forces[i] = 0;
            return 0;
        }

        i++;
    }

    return -1;
}

int Sprite_removeAllForces(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;
    if (_forces == 0) return 0;

    int id = sprite->uid - 1;
    CmdFX_Vector** forces = _forces[id];
    if (forces == 0) return -1;

    for (int i = 0; forces[i] != 0; i++)
        free(forces[i]);

    free(forces);
    _forces[id] = 0;

    return 0;
}

double* _frictionCoefficients = 0;
int _frictionCoefficientsCount = 0;

double Sprite_getFrictionCoefficient(CmdFX_Sprite* sprite) {
    if (sprite == 0) return Engine_getDefaultFrictionCoefficient();
    if (sprite->uid == 0) return Engine_getDefaultFrictionCoefficient();

    if (_frictionCoefficients == 0) return Engine_getDefaultFrictionCoefficient();

    int id = sprite->uid - 1;
    return _frictionCoefficients[id];
}

int Sprite_setFrictionCoefficient(CmdFX_Sprite* sprite, double coefficient) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;
    if (coefficient < 0 || coefficient > 1) return -1;

    int id = sprite->uid - 1;
    if (_frictionCoefficients == 0) {
        _frictionCoefficients = calloc(id + 1, sizeof(double));
        if (_frictionCoefficients == 0) return -1;
    }

    if (_frictionCoefficientsCount < id) {
        double* temp = realloc(_frictionCoefficients, sizeof(double) * (id + 1));
        if (temp == 0) return -1;

        _frictionCoefficients = temp;
        for (int i = _frictionCoefficientsCount; i < id; i++)
            _frictionCoefficients[i] = Engine_getDefaultFrictionCoefficient();

        _frictionCoefficientsCount = id + 1;
    }

    _frictionCoefficients[id] = coefficient;
    return 0;
}

int Sprite_resetFrictionCoefficient(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;

    if (_frictionCoefficients == 0) return 0;

    int id = sprite->uid - 1;
    if (_frictionCoefficientsCount < id) {
        double* temp = realloc(_frictionCoefficients, sizeof(double) * (id + 1));
        if (temp == 0) return -1;

        _frictionCoefficients = temp;
        for (int i = _frictionCoefficientsCount; i < id; i++)
            _frictionCoefficients[i] = Engine_getDefaultFrictionCoefficient();

        _frictionCoefficientsCount = id + 1;
    }

    _frictionCoefficients[id] = Engine_getDefaultFrictionCoefficient();
    return 0;
}
