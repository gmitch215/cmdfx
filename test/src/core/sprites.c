#include <stdio.h>

#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"
#include "../test.h"

int main() {
    int r = 0;

    char** data = Char2DBuilder_createFilled(5, 5, '&');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
    
    r |= assert(sprite->width == 5);
    r |= assert(sprite->height == 5);
    r |= assert(sprite->z == 0);
    r |= assert(sprite->id == 0);
    r |= assert(sprite->uid == 1);

    Sprite_draw(4, 4, sprite);

    r |= assert(sprite->x == 4);
    r |= assert(sprite->y == 4);

    Sprite_remove(sprite);

    r |= assert(sprite->id == 0);
    r |= assert(sprite->x == -1);
    r |= assert(sprite->y == -1);

    Sprite_free(sprite);

    return r;
}