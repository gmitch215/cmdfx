#include <stdio.h>

#include "../test.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/costumes.h"
#include "cmdfx/core/sprites.h"

int main() {
    int r = 0;

    char** data1 = Char2DBuilder_createFilled(2, 2, '#');
    CmdFX_Sprite* sprite = Sprite_create(data1, 0, 0);

    CmdFX_SpriteCostumes* c1 = Sprite_createCostumes(sprite, 3);
    r |= assertNotNull(c1);

    char** data2 = Char2DBuilder_createFilled(2, 2, '@');
    char** data3 = Char2DBuilder_createFilled(3, 3, '%');
    char*** ansi = String2DBuilder_createFilled(2, 2, "\033[31m");
    Sprite_setCostumeAt(sprite, 1, data2, 0);
    Sprite_setCostumeAt(sprite, 2, data3, ansi);

    r |= assertCharArraysMatch(Sprite_getCharCostume(sprite, 0), data1);
    r |= assertStringArraysMatch(Sprite_getAnsiCostume(sprite, 0), 0);
    r |= assertCharArraysMatch(Sprite_getCharCostume(sprite, 1), data2);
    r |= assertCharArraysMatch(Sprite_getCharCostume(sprite, 2), data3);
    r |= assertStringArraysMatch(Sprite_getAnsiCostume(sprite, 2), ansi);

    r |= assertEquals(Sprite_switchCostumeTo(sprite, 1), 0);
    r |= assertCharArraysMatch(sprite->data, data2);
    r |= assertEquals(Sprite_switchCostumeTo(sprite, 2), 0);
    r |= assertCharArraysMatch(sprite->data, data3);
    r |= assertStringArraysMatch(sprite->ansi, ansi);
    r |= assertEquals(Sprite_switchCostumeTo(sprite, 0), 0);
    r |= assertCharArraysMatch(sprite->data, data1);
    r |= assertStringArraysMatch(sprite->ansi, 0);

    Sprite_free(sprite);

    return r;
}