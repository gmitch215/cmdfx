#include <stdio.h>
#include <stdlib.h>

#include "../test.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/engine.h"

// exercises the start -> tick -> end lifecycle repeatedly
// Engine_end must join the loop thread before cleanup so there is no
// use after free; designed to run clean under ThreadSanitizer
int main() {
    int r = 0;

    for (int i = 0; i < 5; i++) {
        char** data = Char2DBuilder_createFilled(3, 3, '#');
        CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
        Sprite_draw(1, 1, sprite);

        r |= assertEquals(Engine_start(), 0);
        // a second start must be rejected while running
        r |= assertEquals(Engine_start(), -1);

        // let the physics loop run a handful of ticks
        sleepMillis(50);

        r |= assertEquals(Engine_end(), 0);
        // a second end must be rejected once stopped
        r |= assertEquals(Engine_end(), -1);

        Sprite_free(sprite);
    }

    return r;
}
