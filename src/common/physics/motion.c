#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/mass.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

// 0 - vx
// 1 - vy
// 2 - ax
// 3 - ay
static double** _motion = 0;
static int _motionCount = 0;

#define _SPRITE_MOTION_MUTEX 4
#define _CANVAS_MUTEX 7

void _checkMotion(CmdFX_Sprite* sprite) {
    if (_motion == 0) {
        _motionCount = Canvas_getDrawnSpritesCount();
        _motion = calloc(_motionCount, sizeof(double*));
        if (_motion == 0) return;
    }

    int id = sprite->id - 1;
    if (_motionCount < id) {
        double** temp = realloc(_motion, sizeof(double*) * (id + 1));
        if (temp == 0) return;

        _motion = temp;
        for (int i = _motionCount; i < id; i++)
            _motion[i] = 0;

        _motionCount = id + 1;
    }

    if (_motion[id] == 0) {
        _motion[id] = calloc(4, sizeof(double));
        if (_motion[id] == 0) return;
    }
}

double* Sprite_getMotion(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    return _motion[id];
}

double Sprite_getVelocityX(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;

    return _motion[id][0];
}

int Sprite_setVelocityX(CmdFX_Sprite* sprite, double velocity) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;
    _motion[id][0] = velocity;

    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return 0;
}

double Sprite_getVelocityY(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;

    return _motion[id][1];
}

int Sprite_setVelocityY(CmdFX_Sprite* sprite, double velocity) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;
    _motion[id][1] = velocity;
    
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return 0;
}

double Sprite_getAccelerationX(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;

    return _motion[id][2];
}

int Sprite_setAccelerationX(CmdFX_Sprite* sprite, double acceleration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;
    _motion[id][2] = acceleration;
    
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return 0;
}

double Sprite_getAccelerationY(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;
    
    return _motion[id][3];
}

int Sprite_setAccelerationY(CmdFX_Sprite* sprite, double acceleration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;
    _motion[id][3] = acceleration;

    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return 0;
}

int Sprite_isAboutToCollide(CmdFX_Sprite* sprite1, CmdFX_Sprite* sprite2) {
    if (sprite1 == sprite2) return 0;
    if (sprite1 == 0) return 0;
    if (sprite2 == 0) return 0;

    if (sprite1->id == 0 || sprite2->id == 0) return 0;
    if (sprite1->id == sprite2->id) return 0;

    if (Sprite_isColliding(sprite1, sprite2)) return 1;

    double* motion1 = Sprite_getMotion(sprite1);
    double* motion2 = Sprite_getMotion(sprite2);
    if (motion1 == 0 || motion2 == 0) return 0; // not colliding + no motion = not about to ollide

    double dx1 = motion1[0] + motion1[2];
    double dy1 = motion1[1] + motion1[3];
    double dx2 = motion2[0] + motion2[2];
    double dy2 = motion2[1] + motion2[3];

    if (sprite1->x <= sprite2->x + sprite2->width + dx2 &&
        sprite1->x + sprite1->width + dx1 >= sprite2->x &&
        sprite1->y <= sprite2->y + sprite2->height + dy2 &&
        sprite1->y + sprite1->height + dy1 >= sprite2->y) {
        return 1;
    }
    
    if (sprite2->x <= sprite1->x + sprite1->width + dx1 &&
        sprite2->x + sprite2->width + dx2 >= sprite1->x &&
        sprite2->y <= sprite1->y + sprite1->height + dy1 &&
        sprite2->y + sprite2->height + dy2 >= sprite1->y) {
        return 1;
    }

    return 0;
}

CmdFX_Sprite** Sprite_getAboutToCollideSprites(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;

    CmdFX_Sprite** sprites = Canvas_getDrawnSprites();
    if (sprites == 0) return 0;

    int spriteCount = Canvas_getDrawnSpritesCount();
    if (spriteCount < 2) return 0;

    int collidingCount = 0;
    int allocated = 4;

    CmdFX_Sprite** colliding = calloc(allocated + 1, sizeof(CmdFX_Sprite*));
    if (!colliding) return 0;

    for (int i = 0; i < spriteCount; i++) {
        CmdFX_Sprite* other = sprites[i];
        if (other->id == 0) continue;
        if (other->id == sprite->id) continue;

        if (Sprite_isAboutToCollide(sprite, other)) {
            if (collidingCount >= allocated) {
                allocated *= 2;
                CmdFX_Sprite** temp = realloc(colliding, sizeof(CmdFX_Sprite*) * (allocated + 1));
                if (!temp) {
                    free(colliding);
                    return 0;
                }
                colliding = temp;

                // crealloc
                for (int j = collidingCount; j < allocated + 1; j++) colliding[j] = 0;
            }

            colliding[collidingCount++] = other;
        }
    }

    colliding[collidingCount] = 0;
    return colliding;
}

// Engine Application

static int _motionDebugEnabled = 0;

int Engine_isMotionDebugEnabled() {
    return _motionDebugEnabled;
}

void Engine_enableMotionDebug() {
    _motionDebugEnabled = 1;
}

void Engine_disableMotionDebug() {
    _motionDebugEnabled = 0;
}

// 0 - vx, 1 - vy
static double** _leftovers = 0;
static int _leftoversCount = 0;

void _checkLeftovers(CmdFX_Sprite* sprite) {
    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
    if (_leftovers == 0) {
        _leftoversCount = Canvas_getDrawnSpritesCount();
        _leftovers = calloc(_leftoversCount, sizeof(double*));
        if (_leftovers == 0) return;
    }

    int id = sprite->id - 1;
    if (_leftoversCount < id) {
        double** temp = realloc(_leftovers, sizeof(double*) * (id + 1));
        if (temp == 0) return;

        _leftovers = temp;
        for (int i = _leftoversCount; i < id; i++)
            _leftovers[i] = 0;

        _leftoversCount = id + 1;
    }

    if (_leftovers[id] == 0) {
        _leftovers[id] = calloc(2, sizeof(double));
        if (_leftovers[id] == 0) return;
    }

    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
}

void Engine_applyMotion(CmdFX_Sprite* sprite) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;
    if (_motion == 0) return;
    if (_motionCount < sprite->id) return;
    if (Sprite_isStatic(sprite)) return;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return;

    double terminalVelocity = Engine_getTerminalVelocity();
    int ground = Engine_getGroundY();
    int width = Canvas_getWidth();

    double* motion = _motion[id];
    double vx = motion[0]; double ax = motion[2]; double dx = vx + ax;
    double vy = motion[1]; double ay = motion[3]; double dy = vy + ay;
    
    // Check Terminal Velocity
    dy = clamp_d(dy, -terminalVelocity, terminalVelocity);

    // Check Bounds
    if (sprite->x + dx <= 0) {
        dx = -sprite->x;
    }
    if (width > 0 && sprite->x + sprite->width + dx >= width) {
        dx = width - sprite->width - sprite->x;
    }
    if (sprite->y - dy <= 0) {
        dy = sprite->y;
    }
    if (ground > 0 && sprite->y + sprite->height - dy >= ground) {
        dy = -ground + sprite->height + sprite->y;
    }

    // Check for Leftovers (velocities from previous frame that are decimals)
    int dx0 = (int) floor(dx);
    int dy0 = (int) floor(dy);
    double leftoverX = dx - dx0;
    double leftoverY = dy - dy0;

    if (leftoverX != 0.0 || leftoverY != 0.0) {
        _checkLeftovers(sprite);
        if (_leftovers[id] == 0) return;
        double* leftovers = _leftovers[id];
        leftovers[0] += leftoverX;
        leftovers[1] += leftoverY;

        if (fabs(leftovers[0]) > 1.0) {
            dx0 += (int) floor(leftovers[0]);
            leftovers[0] -= (int) floor(leftovers[0]);
        }

        if (fabs(leftovers[1]) > 1.0) {
            dy0 += (int) floor(leftovers[1]);
            leftovers[1] -= (int) floor(leftovers[1]);
        }
    }

    if (_motionDebugEnabled) {
        double mass = Sprite_getMass(sprite);
        
        CmdFX_tryLockMutex(_CANVAS_MUTEX);

        Canvas_setCursor(3, sprite->id + 1);
        printf("\033[0m");
        printf("sprite #%d | mass: %.2f | vx: %.2f, vy: %.2f, ax: %.2f, ay: %.2f -- dx: %.2f, dy: %.2f -- x: %d -> %.2f, y: %d -> %.2f", sprite->id, mass, motion[0], motion[1], motion[2], motion[3], dx, dy, sprite->x, sprite->x + dx, sprite->y, sprite->y - dy);
        fflush(stdout);
        Canvas_setCursor(0, 0);

        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    // Move Sprite
    Sprite_moveBy(sprite, dx0, -dy0); // reverse dy
    
    motion[0] = dx;
    motion[1] = dy;
}

// Memory Cleanup

int Sprite_resetAllMotion(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
    int id = sprite->id - 1;
    free(_motion[id]); // free(0) is a no-op
    _motion[id] = 0;
    for (int i = id + 1; i < _motionCount; i++) {
        if (_motion[i] == 0) break;
        _motion[i - 1] = _motion[i];
    }
    _motion[_motionCount - 1] = 0;
    _motionCount--;

    if (_motionCount == 0) {
        free(_motion);
        _motion = 0;
    }

    if (_leftovers != 0) {
        free(_leftovers[id]);
        _leftovers[id] = 0;
        for (int i = id + 1; i < _leftoversCount; i++) {
            if (_leftovers[i] == 0) break;
            _leftovers[i - 1] = _leftovers[i];
        }
        _leftovers[_leftoversCount - 1] = 0;
        _leftoversCount--;

        if (_leftoversCount == 0) {
            free(_leftovers);
            _leftovers = 0;
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    
    return 0;
}