#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/mass.h"

CmdFX_Sprite** _staticSprites = 0;
int _staticSpriteCount = 0;

int Sprite_isStatic(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;

    for (int i = 0; i < _staticSpriteCount; i++) {
        if (_staticSprites[i] == sprite) return 1;
    }

    return 0;
}

int Sprite_setStatic(CmdFX_Sprite* sprite, int isStatic) {
    if (sprite == 0) return -1;

    if (isStatic) {
        if (_staticSprites == 0) {
            _staticSprites = calloc(1, sizeof(CmdFX_Sprite*));
            if (_staticSprites == 0) return -1;
        } else {
            CmdFX_Sprite** temp = realloc(_staticSprites, sizeof(CmdFX_Sprite*) * (_staticSpriteCount + 1));
            if (temp == 0) return -1;

            _staticSprites = temp;
        }

        _staticSprites[_staticSpriteCount] = sprite;
        _staticSpriteCount++;
    } else {
        for (int i = 0; i < _staticSpriteCount; i++) {
            if (_staticSprites[i] == sprite) {
                _staticSprites[i] = 0;
                break;
            }
        }
    }

    return 0;
}

// Engine Declarations

int _forceOfGravity = 1;

int Engine_getForceOfGravity() {
    return _forceOfGravity;
}

int Engine_setForceOfGravity(int force) {
    if (force < 0) return -1;

    _forceOfGravity = force;

    return 0;
}

int _terminalVelocity = 10;

int Engine_getTerminalVelocity() {
    return _terminalVelocity;
}

int Engine_setTerminalVelocity(int velocity) {
    if (velocity < 1) return -1;

    _terminalVelocity = velocity;

    return 0;
}

int _groundY = -1;

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

double _defaultFrictionCoefficient = 0.25;

double Engine_getDefaultFrictionCoefficient() {
    return _defaultFrictionCoefficient;
}

int Engine_setDefaultFrictionCoefficient(double coefficient) {
    if (coefficient < 0 || coefficient > 1) return -1;

    _defaultFrictionCoefficient = coefficient;

    return 0;
}

int* _characterMasses = 0;

int Engine_getCharacterMass(char c) {
    if (_characterMasses == 0) return 1;

    return _characterMasses[(int) c] + 1;
}

int Engine_setCharacterMass(char c, int mass) {
    if (mass < 1) return -1;
    
    if (_characterMasses == 0) {
        _characterMasses = calloc(256, sizeof(int));
        if (_characterMasses == 0) return -1;
    }

    _characterMasses[(int) c] = mass - 1;

    return 0;
}

// Engine

int Engine_cleanup() {
    // cleanup loose variables
    if (_characterMasses != 0) free(_characterMasses);

    return 0;
}

void Engine_tick() {
    CmdFX_Sprite** sprites = Canvas_getDrawnSprites();
    int count = Canvas_getDrawnSpritesCount();

    int ground = Engine_getGroundY();
    int terminalVelocity = Engine_getTerminalVelocity();
    int forceOfGravity = Engine_getForceOfGravity();

    for (int i = 0; i < count; i++) {
        CmdFX_Sprite* sprite = sprites[i];

        // Skip Static Sprites
        if (Sprite_isStatic(sprite)) continue;

        int dx = 0, dy = 0;

        // Apply Forces
        CmdFX_Vector* netForce = Sprite_getNetForce(sprite);
        if (netForce != 0) {
            dx = netForce->x;
            dy = netForce->y;
        }
        free(netForce);

        // Apply Friction
        double frictionCoefficient = Sprite_getFrictionCoefficient(sprite);
        if (sprite->y == ground) {
            if (dx > 0) {
                dx -= frictionCoefficient;
                if (dx < 0) dx = 0;
            } else if (dx < 0) {
                dx += frictionCoefficient;
                if (dx > 0) dx = 0;
            }
        }

        // Apply Gravity
        dy -= forceOfGravity;

        // Collision Forces
        CmdFX_Sprite** colliding = Sprite_getCollidingSprites(sprite);
        if (colliding != 0) {
            for (int j = 0; colliding[j] != 0; j++) {
                CmdFX_Sprite* other = colliding[j];
                if (Sprite_isStatic(other)) continue;

                CmdFX_Vector* otherForce = Sprite_getNetForce(other);

                // m1u1 + m2u2 = m1v1 + m2v2
                // v1 = ((m1 – m2)u1 + 2(m2u2)) / (m1 + m2)
                // v2 = ((m2 – m1)u2 + 2(m1u1)) / (m1 + m2)
                
                int m1 = Sprite_getMass(sprite);
                int m2 = Sprite_getMass(other);
                int u2x = otherForce->x;
                int u2y = otherForce->y;
                
                // dx = v1x, dy = v1y
                int v1x = (((m1 - m2) * dx) + (2 * m2 * u2x)) / (m1 + m2);
                int v1y = (((m1 - m2) * dy) + (2 * m2 * u2y)) / (m1 + m2);
                int v2x = (((m2 - m1) * u2x) + (2 * m1 * dx)) / (m1 + m2);
                int v2y = (((m2 - m1) * u2y) + (2 * m1 * dy)) / (m1 + m2);

                dx = v1x;
                dy = v1y;
                
                free(otherForce);
            }
            free(colliding);
        }

        // Check Terminal Velocity
        if (dy > terminalVelocity) dy = terminalVelocity;
        if (dy < -terminalVelocity) dy = -terminalVelocity;

        // Ensure Sprite stays in bounds
        if (sprite->y + dy < ground)
            dy = ground - sprite->y;
        
        // Move Sprite
        if (dx != 0 || dy != 0)
            Sprite_moveBy(sprite, dx, dy);
    }
}