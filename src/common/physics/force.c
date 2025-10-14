#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/util.h"

#define _SPRITE_FORCE_MUTEX 5
CmdFX_Vector*** _forces = 0;
int* _forcesCounts = 0;
int _forcesSize = 0;

CmdFX_Vector** Sprite_getAllForces(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_forces == 0) return 0;
    if (_forcesCounts == 0) return 0;

    int id = sprite->id - 1;
    if (id >= _forcesSize) return 0;
    return _forces[id];
}

int Sprite_getAllForcesCount(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (_forces == 0) return -1;
    if (_forcesCounts == 0) return -1;

    int id = sprite->id - 1;
    if (id >= _forcesSize) return 0;
    return _forcesCounts[id];
}

CmdFX_Vector* Sprite_getNetForce(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_forces == 0) return 0;
    if (_forcesCounts == 0) return 0;

    int id = sprite->id - 1;
    if (id >= _forcesSize) return 0;
    CmdFX_Vector** forces = _forces[id];
    if (forces == 0) return 0;

    CmdFX_Vector* netForce = Vector_create(0, 0);
    if (netForce == 0) return 0;

    int size = _forcesCounts[id];
    for (int i = 0; i < size; i++) {
        CmdFX_Vector* force = forces[i];

        CmdFX_Vector* newForce = Vector_add(netForce, force);
        free(netForce);
        netForce = newForce;
    }

    return netForce;
}

void _checkForceArraysExist() {
    int size = Canvas_getDrawnSpritesCount();

    // initialize or reallocate _forces
    if (_forces == 0) {
        _forces = calloc(size, sizeof(CmdFX_Vector**));
        if (_forces == 0) return;
        _forcesSize = size;
    }
    if (_forcesSize < size) {
        CmdFX_Vector*** temp = realloc(_forces, sizeof(CmdFX_Vector**) * size);
        if (temp == 0) return;

        _forces = temp;
        for (int i = _forcesSize; i < size; i++) _forces[i] = 0; // crealloc
        _forcesSize = size;
    }

    // initialize or reallocate _forcesCounts
    if (_forcesCounts == 0) {
        _forcesCounts = calloc(size, sizeof(int));
        if (_forcesCounts == 0) {
            free(_forces);
            _forces = 0;
            return;
        }
        _forcesSize = size;
    }
    if (_forcesSize < size) {
        int* temp = realloc(_forcesCounts, sizeof(int) * size);
        if (temp == 0) {
            free(_forces);
            _forces = 0;
            return;
        }

        _forcesCounts = temp;
        for (int i = _forcesSize; i < size; i++)
            _forcesCounts[i] = 0; // crealloc
        _forcesSize = size;
    }
}

int Sprite_addForce(CmdFX_Sprite* sprite, CmdFX_Vector* vector) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (vector == 0) return -1;
    if (vector->x == 0 && vector->y == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_FORCE_MUTEX);

    _checkForceArraysExist();
    if (_forces == 0) return -1;
    if (_forcesCounts == 0) return -1;

    int id = sprite->id - 1;

    if (_forces[id] == 0) {
        _forces[id] = malloc(sizeof(CmdFX_Vector*));
        if (_forces[id] == 0) return -1;

        _forces[id][0] = vector;
        _forcesCounts[id] = 1;
    }
    else {
        int i = _forcesCounts[id];
        CmdFX_Vector** temp =
            realloc(_forces[id], sizeof(CmdFX_Vector*) * (i + 1));
        if (temp == 0) return -1;

        _forces[id] = temp;
        _forces[id][i] = vector;
        _forcesCounts[id] = i + 1;
    }

    CmdFX_tryUnlockMutex(_SPRITE_FORCE_MUTEX);
    return 0;
}

int Sprite_removeForce(CmdFX_Sprite* sprite, CmdFX_Vector* vector) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (vector == 0) return -1;
    if (_forces == 0) return -1;
    if (_forcesCounts == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_FORCE_MUTEX);

    int id = sprite->id - 1;
    int i = _forcesCounts[id];
    if (_forces[id] == 0) return -1;
    if (i == 0) return -1;

    if (i == 1 && _forces[id][0] == vector) {
        free(_forces[id]);
        _forces[id] = 0;
        _forcesCounts[id] = 0;
        _forcesSize--;

        CmdFX_tryUnlockMutex(_SPRITE_FORCE_MUTEX);
        return 0;
    }

    for (int j = 0; j < i; j++) {
        if (_forces[id][j] == vector) {
            _forces[id][j] = 0;
            for (int k = j; k < i - 1; k++) _forces[id][k] = _forces[id][k + 1];

            _forces[id][i - 1] = 0;
            _forcesCounts[id]--;

            CmdFX_tryUnlockMutex(_SPRITE_FORCE_MUTEX);
            return 0;
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_FORCE_MUTEX);

    return -1;
}

int Sprite_removeAllForces(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (_forces == 0) return 0;
    if (_forcesCounts == 0) return 0;

    CmdFX_tryLockMutex(_SPRITE_FORCE_MUTEX);

    int id = sprite->id - 1;
    int size = _forcesCounts[id];
    for (int i = 0; i < size; i++) free(_forces[id][i]);
    free(_forces[id]);
    _forces[id] = 0;
    _forcesSize--;

    CmdFX_tryUnlockMutex(_SPRITE_FORCE_MUTEX);

    return 0;
}

int Sprite_clearAllForces() {
    if (_forces == 0) return -1;
    if (_forcesCounts == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_FORCE_MUTEX);

    for (int i = 0; i < _forcesSize; i++) {
        int size = _forcesCounts[i];
        if (_forces[i] != 0)
            for (int j = 0; j < size; j++) free(_forces[i][j]);

        free(_forces[i]);
        _forces[i] = 0;
    }

    free(_forces);
    _forces = 0;

    free(_forcesCounts);
    _forcesCounts = 0;

    CmdFX_tryUnlockMutex(_SPRITE_FORCE_MUTEX);

    return 0;
}

#define _SPRITE_FRICTION_MUTEX 6
double* _frictionCoefficients = 0;
int _frictionCoefficientsCount = 0;

double Sprite_getFrictionCoefficient(CmdFX_Sprite* sprite) {
    if (sprite == 0) return Engine_getDefaultFrictionCoefficient();
    if (sprite->id == 0) return Engine_getDefaultFrictionCoefficient();

    if (_frictionCoefficients == 0)
        return Engine_getDefaultFrictionCoefficient();

    int id = sprite->id - 1;
    if (id >= _frictionCoefficientsCount)
        return Engine_getDefaultFrictionCoefficient();
    return _frictionCoefficients[id];
}

int Sprite_setFrictionCoefficient(CmdFX_Sprite* sprite, double coefficient) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;
    if (coefficient < 0 || coefficient > 1) return -1;

    CmdFX_tryLockMutex(_SPRITE_FRICTION_MUTEX);

    int id = sprite->id - 1;
    if (_frictionCoefficients == 0) {
        _frictionCoefficients = calloc(id + 1, sizeof(double));
        if (_frictionCoefficients == 0) return -1;
    }

    if (_frictionCoefficientsCount <= id) {
        double* temp =
            realloc(_frictionCoefficients, sizeof(double) * (id + 1));
        if (temp == 0) return -1;

        _frictionCoefficients = temp;
        for (int i = _frictionCoefficientsCount; i <= id; i++)
            _frictionCoefficients[i] = Engine_getDefaultFrictionCoefficient();

        _frictionCoefficientsCount = id + 1;
    }

    _frictionCoefficients[id] = coefficient;

    CmdFX_tryUnlockMutex(_SPRITE_FRICTION_MUTEX);
    return 0;
}

int Sprite_resetFrictionCoefficient(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    if (_frictionCoefficients == 0) return 0;

    CmdFX_tryLockMutex(_SPRITE_FRICTION_MUTEX);

    int id = sprite->id - 1;
    if (_frictionCoefficientsCount < id) {
        double* temp =
            realloc(_frictionCoefficients, sizeof(double) * (id + 1));
        if (temp == 0) return -1;

        _frictionCoefficients = temp;
        for (int i = _frictionCoefficientsCount; i < id; i++)
            _frictionCoefficients[i] = Engine_getDefaultFrictionCoefficient();

        _frictionCoefficientsCount = id + 1;
    }

    _frictionCoefficients[id] = Engine_getDefaultFrictionCoefficient();
    CmdFX_tryUnlockMutex(_SPRITE_FRICTION_MUTEX);
    return 0;
}
