#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

int main() {
    int r = 0;

    Engine_setGroundY(0);

    char** data = Char2DBuilder_createFilled(4, 4, '&');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
    Sprite_draw(1, 1, sprite);

    r |= assertEquals(sprite->x, 1);
    r |= assertEquals(sprite->y, 1);

    r |= assertTrue(!Sprite_setVelocityX(sprite, 2.0));
    r |= assertTrue(!Sprite_setVelocityY(sprite, -3.0));
    Engine_applyMotion(sprite);

    r |= assertDoubleEquals(Sprite_getVelocityX(sprite), 2.0);
    r |= assertDoubleEquals(Sprite_getVelocityY(sprite), -3.0);
    r |= assertEquals(sprite->x, 3);
    r |= assertEquals(sprite->y, 4);

    r |= assertTrue(!Sprite_setAccelerationX(sprite, 1.0));
    r |= assertTrue(!Sprite_setAccelerationY(sprite, -2.0));
    Engine_applyMotion(sprite);

    r |= assertDoubleEquals(Sprite_getAccelerationX(sprite), 1.0);
    r |= assertDoubleEquals(Sprite_getAccelerationY(sprite), -2.0);
    r |= assertDoubleEquals(Sprite_getVelocityX(sprite), 3.0);
    r |= assertDoubleEquals(Sprite_getVelocityY(sprite), -5.0);
    r |= assertEquals(sprite->x, 6);
    r |= assertEquals(sprite->y, 9);

    Sprite_free(sprite);

    return r;
}