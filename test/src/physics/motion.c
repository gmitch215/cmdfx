#include <stdio.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"
#include "../test.h"

int main() {
    int r = 0;

    char** data = Char2DBuilder_createFilled(4, 4, '&');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
    Sprite_draw(1, 1, sprite);

    r |= assert(sprite->x == 1);
    r |= assert(sprite->y == 1);

    r |= assert(Sprite_setVelocityX(sprite, 2) == 0);
    r |= assert(Sprite_setVelocityY(sprite, -3) == 0);
    Engine_applyMotion(sprite);

    r |= assert(Sprite_getVelocityX(sprite) == 2);
    r |= assert(Sprite_getVelocityY(sprite) == -3);
    r |= assert(sprite->x == 3);
    r |= assert(sprite->y == 4);

    r |= assert(Sprite_setAccelerationX(sprite, 1) == 0);
    r |= assert(Sprite_setAccelerationY(sprite, -2) == 0);
    Engine_applyMotion(sprite);

    r |= assert(Sprite_getAccelerationX(sprite) == 1);
    r |= assert(Sprite_getAccelerationY(sprite) == -2);
    r |= assert(Sprite_getVelocityX(sprite) == 3);
    r |= assert(Sprite_getVelocityY(sprite) == -5);
    r |= assert(sprite->x == 6);
    r |= assert(sprite->y == 9);

    Sprite_free(sprite);

    return r;
}