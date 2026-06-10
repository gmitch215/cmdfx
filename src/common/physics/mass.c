#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/mass.h"

#define _SPRITE_MASS_MUTEX 9

double Sprite_getDefaultMass(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;

    double mass = 0;
    for (int i = 0; i < sprite->height; i++) {
        if (sprite->data[i] == 0) break;
        for (int j = 0; j < sprite->width; j++) {
            char c = sprite->data[i][j];
            if (c != ' ') mass += Engine_getCharacterMass(c);
        }
    }

    return mass;
}

double* _masses = 0;
int _massesCount = 0;

double Sprite_getMass(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MASS_MUTEX);

    if (_masses == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
        return Sprite_getDefaultMass(sprite);
    }
    if (_massesCount == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
        return Sprite_getDefaultMass(sprite);
    }

    int id = sprite->uid - 1;
    if (id >= _massesCount) {
        CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
        return Sprite_getDefaultMass(sprite);
    }

    double mass = _masses[id];
    CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);

    if (mass == 0) return Sprite_getDefaultMass(sprite);
    return mass;
}

int Sprite_setMass(CmdFX_Sprite* sprite, double mass) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;
    if (mass == -1) return Sprite_resetMass(sprite);
    if (mass < 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MASS_MUTEX);

    int id = sprite->uid - 1;
    if (_masses == 0) {
        _masses = calloc(id + 1, sizeof(double));
        if (_masses == 0) {
            CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
            return -1;
        }
        _massesCount = id + 1;
    }

    if (_massesCount <= id) {
        double* temp = realloc(_masses, sizeof(double) * (id + 1));
        if (temp == 0) {
            CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
            return -1;
        }

        _masses = temp;
        for (int i = _massesCount; i <= id; i++) {
            _masses[i] = 0;
        }

        _massesCount = id + 1;
    }

    _masses[id] = mass;
    CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
    return 0;
}

int Sprite_resetMass(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MASS_MUTEX);

    if (_masses == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
        return 0;
    }
    if (_massesCount == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
        return 0;
    }

    int id = sprite->uid - 1;
    if (id >= _massesCount) {
        CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
        return 0;
    }
    if (_masses[id] == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
        return 0;
    }

    _masses[id] = 0;
    int allZero = 1;
    for (int i = 0; i < _massesCount; i++)
        if (_masses[i] != 0) {
            allZero = 0;
            break;
        }

    if (allZero) {
        free(_masses);
        _masses = 0;
        _massesCount = 0;
    }

    CmdFX_tryUnlockMutex(_SPRITE_MASS_MUTEX);
    return 0;
}