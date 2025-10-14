#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/sprites.h"

int main() {
    int r = 0;

    CmdFX_Sprite* sprite = Sprite_createFilled(5, 5, '&', 0, 0);
    Sprite_draw(4, 4, sprite);

    r |= assertEquals(sprite->x, 4);
    r |= assertEquals(sprite->y, 4);

    Sprite_moveTo(sprite, 2, 2);

    r |= assertEquals(sprite->x, 2);
    r |= assertEquals(sprite->y, 2);

    Sprite_moveBy(sprite, 1, 1);

    r |= assertEquals(sprite->x, 3);
    r |= assertEquals(sprite->y, 3);

    return r;
}