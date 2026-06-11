#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/mass.h"
#include "cmdfx/physics/motion.h"

#define _STATIC_SPRITE_MUTEX 8

static CmdFX_Sprite** _staticSprites = 0;
static int _staticSpriteCount = 0;
static int _staticSpriteCapacity = 0;

int Sprite_isStatic(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;

    CmdFX_tryLockMutex(_STATIC_SPRITE_MUTEX);
    for (int i = 0; i < _staticSpriteCount; i++) {
        if (_staticSprites[i] == sprite) {
            CmdFX_tryUnlockMutex(_STATIC_SPRITE_MUTEX);
            return 1;
        }
    }
    CmdFX_tryUnlockMutex(_STATIC_SPRITE_MUTEX);

    return 0;
}

int Sprite_setStatic(CmdFX_Sprite* sprite, int isStatic) {
    if (sprite == 0) return -1;

    CmdFX_tryLockMutex(_STATIC_SPRITE_MUTEX);

    if (isStatic) {
        // skip if already static
        for (int i = 0; i < _staticSpriteCount; i++) {
            if (_staticSprites[i] == sprite) {
                CmdFX_tryUnlockMutex(_STATIC_SPRITE_MUTEX);
                return 0;
            }
        }

        // grow with a doubling strategy
        if (_staticSpriteCount >= _staticSpriteCapacity) {
            int newCapacity =
                _staticSpriteCapacity == 0 ? 8 : _staticSpriteCapacity * 2;
            CmdFX_Sprite** temp =
                realloc(_staticSprites, sizeof(CmdFX_Sprite*) * newCapacity);
            if (temp == 0) {
                CmdFX_tryUnlockMutex(_STATIC_SPRITE_MUTEX);
                return -1;
            }

            _staticSprites = temp;
            _staticSpriteCapacity = newCapacity;
        }

        _staticSprites[_staticSpriteCount++] = sprite;
    }
    else {
        // find the sprite before shifting
        int found = -1;
        for (int i = 0; i < _staticSpriteCount; i++) {
            if (_staticSprites[i] == sprite) {
                found = i;
                break;
            }
        }

        // not present so nothing to remove
        if (found < 0) {
            CmdFX_tryUnlockMutex(_STATIC_SPRITE_MUTEX);
            return 0;
        }

        for (int j = found; j < _staticSpriteCount - 1; j++)
            _staticSprites[j] = _staticSprites[j + 1];

        _staticSpriteCount--;
        _staticSprites[_staticSpriteCount] = 0;
    }

    CmdFX_tryUnlockMutex(_STATIC_SPRITE_MUTEX);
    return 0;
}

// Engine Declarations

static double _forceOfGravity = 1.0;

double Engine_getForceOfGravity() {
    return _forceOfGravity;
}

int Engine_setForceOfGravity(double force) {
    if (force < 0) return -1;

    _forceOfGravity = force;

    return 0;
}

static double _terminalVelocity = 10;

double Engine_getTerminalVelocity() {
    return _terminalVelocity;
}

int Engine_setTerminalVelocity(double velocity) {
    if (velocity < 0.0) return -1;

    _terminalVelocity = velocity;

    return 0;
}

static int _groundY = -1;

int Engine_getGroundY() {
    if (_groundY == -1) return Canvas_getHeight();

    return _groundY;
}

int Engine_setGroundY(int y) {
    if (y < 0) return -1;

    _groundY = y;

    return 0;
}

// Engine Defaults

static double _defaultFrictionCoefficient = 0.25;

double Engine_getDefaultFrictionCoefficient() {
    return _defaultFrictionCoefficient;
}

int Engine_setDefaultFrictionCoefficient(double coefficient) {
    if (coefficient < 0 || coefficient > 1) return -1;

    _defaultFrictionCoefficient = coefficient;

    return 0;
}

static double* _characterMasses = 0;

double Engine_getCharacterMass(char c) {
    if (_characterMasses == 0) return 1;

    return _characterMasses[(unsigned char) c] + 1.0;
}

int Engine_setCharacterMass(char c, double mass) {
    if (mass < 1) return -1;

    if (_characterMasses == 0) {
        _characterMasses = calloc(256, sizeof(double));
        if (_characterMasses == 0) return -1;
    }

    _characterMasses[(unsigned char) c] = mass - 1;

    return 0;
}

// Engine

int Engine_cleanup() {
    // cleanup loose variables
    if (_characterMasses != 0) {
        free(_characterMasses);
        _characterMasses = 0;
    }

    if (_staticSprites != 0) {
        free(_staticSprites);
        _staticSprites = 0;
        _staticSpriteCount = 0;
        _staticSpriteCapacity = 0;
    }

    Sprite_clearAllForces();

    return 0;
}

// src/common/core/sprites.c
extern void _lockSpritePair(const CmdFX_Sprite* a, const CmdFX_Sprite* b);
extern void _unlockSpritePair(const CmdFX_Sprite* a, const CmdFX_Sprite* b);

CmdFX_Sprite** Engine_tick() {
    CmdFX_Sprite** sprites = Canvas_getDrawnSprites();
    if (sprites == 0) return 0;

    int count = Canvas_getDrawnSpritesCount();
    if (count < 1) return 0;

    int ground = Engine_getGroundY();
    double forceOfGravity = Engine_getForceOfGravity();

    // size for the worst case of every drawn sprite being non static
    CmdFX_Sprite** modified = calloc(count + 1, sizeof(CmdFX_Sprite*));
    if (modified == 0) return 0;

    int c = 0;
    for (int i = 0; i < count; i++) {
        CmdFX_Sprite* sprite = sprites[i];

        // Skip Static Sprites
        if (Sprite_isStatic(sprite)) continue;

        // persist velocity
        double dvx = Sprite_getVelocityX(sprite); // Δvx
        double dvy = Sprite_getVelocityY(sprite); // Δvy

        // don't persist acceleration
        double dax = 0.0; // Δax
        double day = 0.0; // Δay

        // Apply Forces
        CmdFX_Vector* netForce = Sprite_getNetForce(sprite);
        if (netForce != 0) {
            dax = netForce->x;
            day = netForce->y;
        }
        free(netForce);

        // Apply Gravity
        if (ground == 0 || sprite->y + sprite->height < ground)
            day -= forceOfGravity;

        CmdFX_Sprite** colliding = Sprite_getCollidingSprites(sprite);
        if (colliding != 0) {
            for (int j = 0; colliding[j] != 0; j++) {
                CmdFX_Sprite* other = colliding[j];
                if (Sprite_isStatic(other)) continue;

                if (sprite->id > other->id) continue;

                _lockSpritePair(sprite, other);

                // m1u1 + m2u2 = m1v1 + m2v2
                // v1 = ((m1 – m2)u1 + 2(m2u2)) / (m1 + m2)
                // v2 = ((m2 – m1)u2 + 2(m1u1)) / (m1 + m2)

                double m1 = Sprite_getMass(sprite);
                double m2 = Sprite_getMass(other);
                double u2x = Sprite_getVelocityX(other);
                double u2y = Sprite_getVelocityY(other);

                // dx = v1x, dy = v1y
                double v1x = (((m1 - m2) * dvx) + (2.0 * m2 * u2x)) / (m1 + m2);
                double v1y = (((m1 - m2) * dvy) + (2.0 * m2 * u2y)) / (m1 + m2);
                double v2x = (((m2 - m1) * u2x) + (2.0 * m1 * dvx)) / (m1 + m2);
                double v2y = (((m2 - m1) * u2y) + (2.0 * m1 * dvy)) / (m1 + m2);

                dvx = v1x;
                dvy = v1y;
                Sprite_setVelocityX(other, v2x);
                Sprite_setVelocityY(other, v2y);

                _unlockSpritePair(sprite, other);
            }
        }
        free(colliding);

        // Apply friction only if ground is active (ground > 0) and sprite is
        // on/at ground
        double frictionCoefficient = Sprite_getFrictionCoefficient(sprite);
        if (ground > 0 && sprite->y + sprite->height >= ground && dvx != 0) {
            if (dvx > 0) {
                dax -= frictionCoefficient * forceOfGravity;
                if (dvx + dax < 0)
                    dax = -dvx; // ensure no negative acceleration
            }
            else if (dvx < 0) {
                dax += frictionCoefficient * forceOfGravity;
                if (dvx + dax > 0)
                    dax = -dvx; // ensure no negative acceleration
            }
        }

        // Apply Motion
        Sprite_setVelocityX(sprite, dvx);
        Sprite_setVelocityY(sprite, dvy);
        Sprite_setAccelerationX(sprite, dax);
        Sprite_setAccelerationY(sprite, day);

        modified[c++] = sprite;
    }

    modified[c] = 0;
    return modified;
}
