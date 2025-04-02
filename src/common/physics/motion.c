#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

// 0 - vx
// 1 - vy
// 2 - ax
// 3 - ay
static int** _motion = 0;
static int _motionCount = 0;

void _checkMotion(CmdFX_Sprite* sprite) {
    if (_motion == 0) {
        _motionCount = Canvas_getDrawnSpritesCount();
        _motion = calloc(_motionCount, sizeof(int*));
        if (_motion == 0) return;
    }

    int id = sprite->id - 1;
    if (_motionCount < id) {
        int** temp = realloc(_motion, sizeof(int*) * (id + 1));
        if (temp == 0) return;

        _motion = temp;
        for (int i = _motionCount; i < id; i++)
            _motion[i] = 0;

        _motionCount = id + 1;
    }

    if (_motion[id] == 0) {
        _motion[id] = calloc(4, sizeof(int));
        if (_motion[id] == 0) return;
    }
}

int* Sprite_getMotion(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    return _motion[id];
}

int Sprite_getVelocityX(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;

    return _motion[id][0];
}

int Sprite_setVelocityX(CmdFX_Sprite* sprite, int velocity) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    _motion[id][0] = velocity;
    return 0;
}

int Sprite_getVelocityY(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;

    return _motion[id][1];
}

int Sprite_setVelocityY(CmdFX_Sprite* sprite, int velocity) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    _motion[id][1] = velocity;
    return 0;
}

int Sprite_getAccelerationX(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;

    return _motion[id][2];
}

int Sprite_setAccelerationX(CmdFX_Sprite* sprite, int acceleration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    _motion[id][2] = acceleration;
    return 0;
}

int Sprite_getAccelerationY(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_motion == 0) return 0;
    if (_motionCount < sprite->id) return 0;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return 0;

    return _motion[id][3];
}

int Sprite_setAccelerationY(CmdFX_Sprite* sprite, int acceleration) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    _checkMotion(sprite);
    if (_motion == 0) return -1;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return -1;

    _motion[id][3] = acceleration;
    return 0;
}

int Sprite_resetAllMotion(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->id == 0) return -1;

    _checkMotion(sprite);
    if (_motion == 0) return -1;

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
    
    return 0;
}

// Engine Application

void Engine_applyMotion(CmdFX_Sprite* sprite) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;
    if (_motion == 0) return;
    if (_motionCount < sprite->id) return;
    if (Sprite_isStatic(sprite)) return;

    int id = sprite->id - 1;
    if (_motion[id] == 0) return;

    int terminalVelocity = Engine_getTerminalVelocity();
    int ground = Engine_getGroundY();

    int* motion = _motion[id];
    int vx = motion[0]; int ax = motion[2]; int dx = vx + ax;
    int vy = motion[1]; int ay = motion[3]; int dy = vy + ay;
    
    // Check Terminal Velocity
    dy = clamp_i(dy, -terminalVelocity, terminalVelocity);

    // Check Ground
    if (ground > 0 && sprite->y - dy >= ground) { // dy is reversed
        dy = ground - sprite->y;
        motion[3] = 0; // reset y acceleration
    }

    // Move Sprite
    if (dx != 0 || dy != 0)
        Sprite_moveBy(sprite, dx, -dy); // reverse dy
    
    motion[0] = dx;
    motion[1] = dy;
}