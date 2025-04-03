#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/motion.h"
#include "cmdfx/physics/engine.h"
#include "../test.h"

void tick(CmdFX_Sprite* sprite) {
    free(Engine_tick());
    Engine_applyMotion(sprite);
}

int main() {
    int r = 0;

    CmdFX_Sprite* sprite = Sprite_create(0, 0, 0);
    r |= assertFalse(Sprite_isStatic(sprite));
    r |= assertTrue(!Sprite_setStatic(sprite, 1));
    r |= assertTrue(Sprite_isStatic(sprite));
    r |= assertTrue(!Sprite_setStatic(sprite, 0));
    r |= assertFalse(Sprite_isStatic(sprite));
    Sprite_free(sprite);

    r |= assertTrue(!Engine_setForceOfGravity(2));
    r |= assertEquals(Engine_getForceOfGravity(), 2);
    r |= assertTrue(!Engine_setForceOfGravity(1));
    r |= assertEquals(Engine_getForceOfGravity(), 1);

    r |= assertTrue(!Engine_setTerminalVelocity(12));
    r |= assertEquals(Engine_getTerminalVelocity(), 12);
    r |= assertTrue(!Engine_setTerminalVelocity(10));
    r |= assertEquals(Engine_getTerminalVelocity(), 10);

    r |= assertTrue(!Engine_setGroundY(5));
    r |= assertEquals(Engine_getGroundY(), 5);
    r |= assertTrue(!Engine_setGroundY(0));   
    r |= assertEquals(Engine_getGroundY(), 0);

    r |= assertTrue(!Engine_setDefaultFrictionCoefficient(0.5));
    r |= assertDoubleEquals(Engine_getDefaultFrictionCoefficient(), 0.5);
    r |= assertTrue(!Engine_setDefaultFrictionCoefficient(0.25));
    r |= assertDoubleEquals(Engine_getDefaultFrictionCoefficient(), 0.25);

    // Engine Tick
    char** data = Char2DBuilder_createFilled(5, 5, '@');
    CmdFX_Sprite* square = Sprite_create(data, 0, 0);
    Sprite_draw(1, 10, square);

    r |= assertEquals(square->y, 10);
    tick(square);
    r |= assertEquals(square->y, 11);
    for (int i = 0; i < 10; i++) tick(square);
    r |= assertEquals(square->y, 75);

    r |= assertEquals(square->x, 1);
    CmdFX_Vector* force = Vector_create(2, 0); // ax = 2, ay = -1
    Sprite_addForce(square, force);
    for (int i = 0; i < 10; i++) tick(square);
    r |= assertEquals(square->x, 99);
    r |= assertEquals(square->y, 175);

    Sprite_removeForce(square, force); // x velocity remains as 20
    for (int i = 0; i < 10; i++) tick(square);
    r |= assertEquals(square->x, 263);
    r |= assertEquals(square->y, 275);

    Sprite_free(square);

    return r;
}