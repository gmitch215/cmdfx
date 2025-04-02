#include <stdio.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/physics/mass.h"
#include "cmdfx/physics/engine.h"
#include "../test.h"

int main() {
    int r = 0;

    char** data = Char2DBuilder_create(3, 3);
    Char2DBuilder_fill(data, 'X');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
    Sprite_draw(2, 2, sprite);

    r |= assertEquals(Sprite_getDefaultMass(sprite), 9);

    Engine_setCharacterMass('X', 2);
    
    r |= assertEquals(Sprite_getDefaultMass(sprite), 18);

    Sprite_setMass(sprite, 10);
    Sprite_getMass(sprite);

    r |= assertEquals(Sprite_getMass(sprite), 10);

    Sprite_resetMass(sprite);

    r |= assertEquals(Sprite_getMass(sprite), 18);

    Sprite_free(sprite);
    Engine_cleanup();
    
    return r;
}