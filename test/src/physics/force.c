#include <stdio.h>
#include <stdlib.h>

#include "../test.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/force.h"

int main() {
    int r = 0;

    char** data = Char2DBuilder_createFilled(3, 3, '#');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
    Sprite_draw(2, 2, sprite);

    r |= assertNull(Sprite_getAllForces(sprite));
    Sprite_addForce(sprite, Vector_create(1, 1));
    CmdFX_Vector* net = Sprite_getNetForce(sprite);
    r |= assertEquals(net->x, 1);
    r |= assertEquals(net->y, 1);
    free(net);
    Sprite_removeAllForces(sprite);

    r |= assertDoubleEquals(Sprite_getFrictionCoefficient(sprite), 0.25);
    r |= assertTrue(!Sprite_setFrictionCoefficient(sprite, 0.5));
    r |= assertDoubleEquals(Sprite_getFrictionCoefficient(sprite), 0.5);
    r |= assertTrue(!Sprite_resetFrictionCoefficient(sprite));
    r |= assertDoubleEquals(Sprite_getFrictionCoefficient(sprite), 0.25);

    Sprite_free(sprite);

    return r;
}