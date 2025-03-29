#include <stdio.h>

#include "cmdfx/core/sprites.h"
#include "../test.h"

int main() {
    int r = 0;

    CmdFX_Sprite* sprite = Sprite_createFilled(5, 5, '&', 0, 0);
    Sprite_draw(4, 4, sprite);

    r |= assert(sprite->x == 4);
    r |= assert(sprite->y == 4);

    Sprite_moveTo(sprite, 2, 2);

    r |= assert(sprite->x == 2);
    r |= assert(sprite->y == 2);

    Sprite_moveBy(sprite, 1, 1);

    r |= assert(sprite->x == 3);
    r |= assert(sprite->y == 3);

    return r;
}