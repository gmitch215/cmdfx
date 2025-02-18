#include <stdio.h>

#include "cmdfx/core/sprites.h"
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

    return r;
}