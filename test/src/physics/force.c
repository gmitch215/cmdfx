#include <stdio.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/physics/force.h"
#include "../test.h"

int main() {
    int r = 0;

    char** data = CharBuilder_createFilled(3, 3, '#');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
    Sprite_draw(2, 2, sprite);

    r |= assert(Sprite_getAllForces(sprite) == 0);
    Sprite_addForce(sprite, Vector_create(1, 1));
    CmdFX_Vector* net = Sprite_getNetForce(sprite);
    r |= assert(net->x == 1);
    r |= assert(net->y == 1);
    free(net);
    Sprite_removeAllForces(sprite);

    r |= assert(Sprite_getFrictionCoefficient(sprite) == 0.25);
    r |= assert(!Sprite_setFrictionCoefficient(sprite, 0.5));
    r |= assert(Sprite_getFrictionCoefficient(sprite) == 0.5);
    r |= assert(!Sprite_resetFrictionCoefficient(sprite));
    r |= assert(Sprite_getFrictionCoefficient(sprite) == 0.25);

    Sprite_free(sprite);

    return r;
}