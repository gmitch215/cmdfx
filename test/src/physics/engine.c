#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/engine.h"
#include "../test.h"

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
    char** data = CharBuilder_createFilled(5, 5, '@');
    CmdFX_Sprite* square = Sprite_create(data, 0, 0);
    Sprite_draw(1, 10, square);

    r |= assert(square->y == 10);
    Engine_tick();
    r |= assert(square->y == 9);
    for (int i = 0; i < 10; i++) Engine_tick();
    r |= assert(square->y == 0);
    Engine_tick();
    r |= assert(square->y == 0);

    r |= assert(square->x == 1);
    CmdFX_Vector* impulse = Vector_create(2, 0);
    Sprite_addForce(square, impulse);
    for (int i = 0; i < 10; i++) Engine_tick();
    r |= assert(square->x == 11);
    Sprite_removeForce(square, impulse);
    for (int i = 0; i < 10; i++) Engine_tick();
    r |= assert(square->x == 11);
    free(impulse);

    Sprite_free(square);

    return r;
}