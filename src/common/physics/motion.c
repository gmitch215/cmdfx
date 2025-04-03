#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

// 0 - vx
// 1 - vy
// 2 - ax
// 3 - ay
static double** _motion = 0;
static int _motionCount = 0;

#define _SPRITE_MOTION_MUTEX 3

void _checkMotion(CmdFX_Sprite* sprite) {
    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
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
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
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

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
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

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
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

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
    return _motion[id][2];
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
}

int Sprite_setAccelerationX(CmdFX_Sprite* sprite, double acceleration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
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

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
    _motion[id][3] = acceleration;
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return 0;
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

    double lvx = 0.0, lvy = 0.0;
    if (leftoverX != 0.0 || leftoverY != 0.0) {
        _checkLeftovers(sprite);
        if (_leftovers[id] == 0) return;
        double* leftovers = _leftovers[id];
        leftovers[0] += leftoverX;
        leftovers[1] += leftoverY;
        lvx = leftovers[0];
        lvy = leftovers[1];

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
        Canvas_setCursor(3, sprite->id + 1);
        printf("sprite #%d | vx: %.2f, vy: %.2f, ax: %.2f, ay: %.2f -- dx: %.2f, dy: %.2f -- x: %d -> %.2f, y: %d -> %.2f -- lvx: %.2f, lvy: %.2f", sprite->id, motion[0], motion[1], motion[2], motion[3], dx, dy, sprite->x, sprite->x + dx, sprite->y, sprite->y - dy, lvx, lvy);
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