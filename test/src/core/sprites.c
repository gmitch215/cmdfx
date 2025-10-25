#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"

int main() {
    int r = 0;

    char** data = Char2DBuilder_createFilled(5, 5, '&');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);

    r |= assertEquals(sprite->width, 5);
    r |= assertEquals(sprite->height, 5);
    r |= assertEquals(sprite->z, 0);
    r |= assertEquals(sprite->id, 0);
    r |= assertEquals(sprite->uid, 1);

    Sprite_draw(4, 4, sprite);

    r |= assertEquals(sprite->x, 4);
    r |= assertEquals(sprite->y, 4);

    Sprite_remove(sprite);

    r |= assertEquals(sprite->id, 0);
    r |= assertEquals(sprite->x, -1);
    r |= assertEquals(sprite->y, -1);

    Sprite_free(sprite);

    return r;
}