#include <stdio.h>
#include <math.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/animation/sprites.h"
#include "cmdfx/core/util.h"

// Utility Methods - Movement

void Sprite_moveTo_anim(CmdFX_Sprite* sprite, int x, int y, double time) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;
    if (x < 0 || y < 0) return;
    if (time <= 0) return;

    int dx = x - sprite->x;
    int dy = y - sprite->y;

    double distance = sqrt(dx * dx + dy * dy);
    double speed = distance / (time * 1000);

    double stepX = dx / distance;
    double stepY = dy / distance;

    double stepTime = 1 / speed;

    double currentTime = 0;
    while (currentTime < time) {
        Sprite_moveBy(sprite, stepX, stepY);

        currentTime += stepTime;
        sleepMillis(stepTime);
    }

    Sprite_moveTo(sprite, x, y);
}

void Sprite_moveBy_anim(CmdFX_Sprite* sprite, int dx, int dy, double time) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;
    if (time <= 0) return;

    Sprite_moveTo_anim(sprite, sprite->x + dx, sprite->y + dy, time);
}

