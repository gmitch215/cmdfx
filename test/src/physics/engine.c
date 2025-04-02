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
    r |= assert(!Sprite_isStatic(sprite));
    r |= assert(!Sprite_setStatic(sprite, 1));
    r |= assert(Sprite_isStatic(sprite));
    r |= assert(!Sprite_setStatic(sprite, 0));
    r |= assert(!Sprite_isStatic(sprite));
    Sprite_free(sprite);

    r |= assert(!Engine_setForceOfGravity(2));
    r |= assert(Engine_getForceOfGravity() == 2);
    r |= assert(!Engine_setForceOfGravity(1));
    r |= assert(Engine_getForceOfGravity() == 1);

    r |= assert(!Engine_setTerminalVelocity(12));
    r |= assert(Engine_getTerminalVelocity() == 12);
    r |= assert(!Engine_setTerminalVelocity(10));
    r |= assert(Engine_getTerminalVelocity() == 10);

    r |= assert(!Engine_setGroundY(5));
    r |= assert(Engine_getGroundY() == 5);
    r |= assert(!Engine_setGroundY(0));   
    r |= assert(Engine_getGroundY() == 0);

    r |= assert(!Engine_setDefaultFrictionCoefficient(0.5));
    r |= assert(Engine_getDefaultFrictionCoefficient() == 0.5);
    r |= assert(!Engine_setDefaultFrictionCoefficient(0.25));
    r |= assert(Engine_getDefaultFrictionCoefficient() == 0.25);

    // Engine Tick
    char** data = Char2DBuilder_createFilled(5, 5, '@');
    CmdFX_Sprite* square = Sprite_create(data, 0, 0);
    Sprite_draw(1, 10, square);

    r |= assert(square->y == 10);
    tick(square);
    r |= assert(square->y == 11);
    for (int i = 0; i < 10; i++) tick(square);
    r |= assert(square->y == 75);

    r |= assert(square->x == 1);
    CmdFX_Vector* force = Vector_create(2, 0); // ax = 2, ay = -1
    Sprite_addForce(square, force);
    for (int i = 0; i < 10; i++) tick(square);
    r |= assert(square->x == 111);
    r |= assert(square->y == 175);

    Sprite_removeForce(square, force); // x velocity remains as 20
    for (int i = 0; i < 10; i++) tick(square);
    r |= assert(square->x == 311);
    r |= assert(square->y == 275);

    Sprite_free(square);

    return r;
}