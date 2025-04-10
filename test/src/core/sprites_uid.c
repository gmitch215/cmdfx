#include <stdio.h>

#include "cmdfx/core/sprites.h"
#include "../test.h"

int main() {
    int r = 0;

    CmdFX_Sprite* sprite1 = Sprite_create(0, 0, 0);
    r |= assertEquals(sprite1->uid, 1);

    CmdFX_Sprite* sprite2 = Sprite_create(0, 0, 0);
    r |= assertEquals(sprite2->uid, 2);

    Sprite_free(sprite1);

    CmdFX_Sprite* sprite3 = Sprite_create(0, 0, 0);
    r |= assertEquals(sprite3->uid, 1);

    Sprite_free(sprite2);
    Sprite_free(sprite3);

    CmdFX_Sprite* sprite4 = Sprite_create(0, 0, 0);
    CmdFX_Sprite* sprite5 = Sprite_create(0, 0, 0);
    CmdFX_Sprite* sprite6 = Sprite_create(0, 0, 0);
    CmdFX_Sprite* sprite7 = Sprite_create(0, 0, 0);

    r |= assertEquals(sprite4->uid, 1);
    r |= assertEquals(sprite5->uid, 2);
    r |= assertEquals(sprite6->uid, 3);
    r |= assertEquals(sprite7->uid, 4);

    Sprite_free(sprite5);

    CmdFX_Sprite* sprite8 = Sprite_create(0, 0, 0);
    r |= assertEquals(sprite8->uid, 2);

    Sprite_free(sprite7);

    CmdFX_Sprite* sprite9 = Sprite_create(0, 0, 0);
    r |= assertEquals(sprite9->uid, 4);

    Sprite_free(sprite4);
    Sprite_free(sprite6);
    Sprite_free(sprite8);
    Sprite_free(sprite9);

    return r;
}