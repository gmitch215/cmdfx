#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/mass.h"
#include "cmdfx/physics/motion.h"
#include "common/core/curses_backend.h"

// 0 - vx
// 1 - vy
// 2 - ax
// 3 - ay
static double** _motion = 0;
static int _motionCount = 0;

// Mutex ID Allocation:
// 0: _SPRITE_DRAWN_MUTEX
// 1: _SPRITE_UID_MUTEX
// 2-3: Reserved (POSITION and DATA)
// 4: _SPRITE_MOTION_MUTEX
// 5: _SPRITE_FORCE_MUTEX
// 6: _SPRITE_FRICTION_MUTEX
// 7: _CANVAS_MUTEX
// 8: _STATIC_SPRITE_MUTEX
// 9: _SPRITE_MASS_MUTEX
// 10: Reserved
// 11-127: Per-sprite mutexes (117 available)
#define _SPRITE_MOTION_MUTEX 4
#define _CANVAS_MUTEX 7

void _checkMotion(CmdFX_Sprite* sprite) {
    if (_motion == 0) {
        _motionCount = Canvas_getDrawnSpritesCount();
        _motion = calloc(_motionCount, sizeof(double*));
        if (_motion == 0) return;
    }

    int id = sprite->id - 1;
    if (id >= _motionCount) {
        double** temp = realloc(_motion, sizeof(double*) * (id + 1));
        if (temp == 0) return;

        _motion = temp;
        for (int i = _motionCount; i <= id; i++) _motion[i] = 0;

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

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    int id = sprite->id - 1;
    if (id >= _motionCount) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }

    if (_motion[id] == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }

    // Return a copy to avoid race conditions when caller accesses the data
    double* copy = malloc(4 * sizeof(double));
    if (copy != 0) {
        copy[0] = _motion[id][0];
        copy[1] = _motion[id][1];
        copy[2] = _motion[id][2];
        copy[3] = _motion[id][3];
    }

    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return copy;
}

double Sprite_getVelocityX(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    int id = sprite->id - 1;
    if (id >= _motionCount) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }
    if (_motion[id] == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }

    double result = _motion[id][0];
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return result;
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

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    int id = sprite->id - 1;
    if (id >= _motionCount) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }
    if (_motion[id] == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }

    double result = _motion[id][1];
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return result;
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

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    int id = sprite->id - 1;
    if (id >= _motionCount) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }
    if (_motion[id] == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }

    double result = _motion[id][2];
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return result;
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

    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);

    int id = sprite->id - 1;
    if (id >= _motionCount) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }
    if (_motion[id] == 0) {
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
        return 0;
    }

    double result = _motion[id][3];
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    return result;
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
    if (motion1 == 0 || motion2 == 0) {
        free(motion1);
        free(motion2);
        return 0; // not colliding + no motion = not about to collide
    }

    // next-step displacement of each sprite
    double dx1 = motion1[0] + motion1[2];
    double dy1 = motion1[1] + motion1[3];
    double dx2 = motion2[0] + motion2[2];
    double dy2 = motion2[1] + motion2[3];
    free(motion1);
    free(motion2);

    // sweep sprite1 against a stationary sprite2 using their relative motion;
    // a collision is imminent only if sprite1's swept box over one step still
    // overlaps sprite2, so diverging or merely passing paths never trigger
    double rdx = dx1 - dx2;
    double rdy = dy1 - dy2;

    double minX = sprite1->x + (rdx < 0 ? rdx : 0);
    double maxX = sprite1->x + sprite1->width + (rdx > 0 ? rdx : 0);
    double minY = sprite1->y + (rdy < 0 ? rdy : 0);
    double maxY = sprite1->y + sprite1->height + (rdy > 0 ? rdy : 0);

    return minX <= sprite2->x + sprite2->width && maxX >= sprite2->x &&
           minY <= sprite2->y + sprite2->height && maxY >= sprite2->y;
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
                CmdFX_Sprite** temp =
                    realloc(colliding, sizeof(CmdFX_Sprite*) * (allocated + 1));
                if (!temp) {
                    free(colliding);
                    return 0;
                }
                colliding = temp;

                // crealloc
                for (int j = collidingCount; j < allocated + 1; j++)
                    colliding[j] = 0;
            }

            colliding[collidingCount++] = other;
        }
    }

    colliding[collidingCount] = 0;
    return colliding;
}

// Engine Application

// src/common/core/sprites.c
extern void _lockSprite(const CmdFX_Sprite* a);
extern void _unlockSprite(const CmdFX_Sprite* a);

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
    if (_leftovers == 0) {
        _leftoversCount = Canvas_getDrawnSpritesCount();
        _leftovers = calloc(_leftoversCount, sizeof(double*));
        if (_leftovers == 0) {
            return;
        }
    }

    int id = sprite->id - 1;
    if (id >= _leftoversCount) {
        double** temp = realloc(_leftovers, sizeof(double*) * (id + 1));
        if (temp == 0) {
            return;
        }

        _leftovers = temp;
        for (int i = _leftoversCount; i <= id; i++) _leftovers[i] = 0;

        _leftoversCount = id + 1;
    }

    if (_leftovers[id] == 0) {
        _leftovers[id] = calloc(2, sizeof(double));
        if (_leftovers[id] == 0) {
            return;
        }
    }
}

// conservative advancement: clamp a world-space integer move so this sprite
// stops flush against another non-static sprite instead of overshooting into
// or tunneling through it; the elastic response then resolves on contact
void _clampMoveToContact(CmdFX_Sprite* sprite, int* wdx, int* wdy) {
    CmdFX_Sprite** sprites = Canvas_getDrawnSprites();
    if (sprites == 0) return;

    int count = Canvas_getDrawnSpritesCount();
    for (int i = 0; i < count; i++) {
        CmdFX_Sprite* o = sprites[i];
        if (o == 0 || o == sprite) continue;
        if (o->id == 0 || o->id == sprite->id) continue;
        if (Sprite_isStatic(o)) continue;

        // x axis, only while the rows currently overlap
        if (*wdx != 0 && sprite->y < o->y + o->height &&
            sprite->y + sprite->height > o->y) {
            if (*wdx > 0 && o->x >= sprite->x + sprite->width) {
                int gap = o->x - (sprite->x + sprite->width);
                if (*wdx > gap) *wdx = gap;
            }
            else if (*wdx < 0 && o->x + o->width <= sprite->x) {
                int gap = sprite->x - (o->x + o->width);
                if (-*wdx > gap) *wdx = -gap;
            }
        }

        // y axis, only while the columns currently overlap
        if (*wdy != 0 && sprite->x < o->x + o->width &&
            sprite->x + sprite->width > o->x) {
            if (*wdy > 0 && o->y >= sprite->y + sprite->height) {
                int gap = o->y - (sprite->y + sprite->height);
                if (*wdy > gap) *wdy = gap;
            }
            else if (*wdy < 0 && o->y + o->height <= sprite->y) {
                int gap = sprite->y - (o->y + o->height);
                if (-*wdy > gap) *wdy = -gap;
            }
        }
    }
}

void Engine_applyMotion(CmdFX_Sprite* sprite) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;
    if (_motion == 0) return;
    if (Sprite_isStatic(sprite)) return;

    // Per-sprite lock while reading/updating motion and moving
    _lockSprite(sprite);

    int id = sprite->id - 1;
    if (id >= _motionCount) {
        _unlockSprite(sprite);
        return;
    }
    if (_motion[id] == 0) {
        _unlockSprite(sprite);
        return;
    }

    double terminalVelocity = Engine_getTerminalVelocity();
    int ground = Engine_getGroundY();
    int width = Canvas_getWidth();

    // atomically read current motion state
    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
    double* motion = _motion[id];
    double vx = motion[0];
    double ax = motion[2];
    double vy = motion[1];
    double ay = motion[3];
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);

    double dx = vx + ax;
    double dy = vy + ay;

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
        CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
        _checkLeftovers(sprite);
        if (_leftovers[id] != 0) {
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
        CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);
    }

    if (_motionDebugEnabled) {
        double mass = Sprite_getMass(sprite);

        // a raw printf bypasses the curses backend and scrolls the screen into
        // a flow of output, so format the line and stamp it through curses
        char buf[192];
        int len = snprintf(
            buf, sizeof(buf),
            "sprite #%d | mass: %.2f | vx: %.2f, vy: %.2f, ax: %.2f, ay: "
            "%.2f -- dx: %.2f, dy: %.2f -- x: %d -> %.2f, y: %d -> %.2f",
            sprite->id, mass, vx, vy, ax, ay, dx, dy, sprite->x, sprite->x + dx,
            sprite->y, sprite->y - dy
        );
        if (len < 0) len = 0;

        // pad so a shorter line overwrites the previous frame in place
        while (len < 140 && len < (int) sizeof(buf) - 1) buf[len++] = ' ';
        buf[len] = 0;

        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        int row = sprite->id + 1;
        CmdFX_curses_resetAttributes();
        for (int i = 0; i < len; i++)
            CmdFX_curses_putCharAt(3 + i, row, buf[i]);
        CmdFX_curses_refresh();
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    // Move Sprite, clamped so it stops flush against another sprite rather
    // than reversing a full tick early or tunneling through it
    int wdx = dx0;
    int wdy = -dy0; // world delta (y grows downward); reverse dy
    _clampMoveToContact(sprite, &wdx, &wdy);
    Sprite_moveBy(sprite, wdx, wdy);

    // atomically update motion state for next frame
    CmdFX_tryLockMutex(_SPRITE_MOTION_MUTEX);
    if (_motion[id] != 0) {
        _motion[id][0] = vx + ax;
        _motion[id][1] = vy + ay;
    }
    CmdFX_tryUnlockMutex(_SPRITE_MOTION_MUTEX);

    _unlockSprite(sprite);
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
