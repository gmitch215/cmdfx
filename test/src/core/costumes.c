#include <stdio.h>

#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/costumes.h"
#include "../test.h"

int main() {
    int r = 0;

    CmdFX_Sprite* sprite = Sprite_create(
        Char2DBuilder_createFilled(2, 2, '#'),
        0, 0
    );

    CmdFX_SpriteCostumes* c1 = Sprite_createCostumes(sprite, 3);
    r |= assert(c1 != 0); 

    Sprite_setCostumeAt(sprite, 1, Char2DBuilder_createFilled(2, 2, '@'), 0);
    Sprite_setCostumeAt(sprite, 2, Char2DBuilder_createFilled(3, 3, '%'), String2DBuilder_createFilled(2, 2, "\033[31m"));

    return r;
}