#include <stdio.h>
#include <stdlib.h>

#include "../test.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

// runs one fully deterministic scenario and writes the final position
// integration is applied synchronously so the result must never vary
static void runScenario(int* outX, int* outY) {
    Engine_setForceOfGravity(1);
    Engine_setTerminalVelocity(100);
    Engine_setGroundY(0);

    char** data = Char2DBuilder_createFilled(3, 3, '#');
    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);
    Sprite_draw(1, 5, sprite);
    Sprite_setVelocityX(sprite, 2);
    Sprite_setVelocityY(sprite, 0);

    for (int i = 0; i < 25; i++) {
        free(Engine_tick());
        Engine_applyMotion(sprite);
    }

    *outX = sprite->x;
    *outY = sprite->y;

    Sprite_free(sprite);
    Engine_cleanup();
}

int main() {
    int r = 0;

    int baseX = 0, baseY = 0;
    int mismatches = 0;

    // identical inputs must yield identical positions every single run
    for (int run = 0; run < 200; run++) {
        int x = 0, y = 0;
        runScenario(&x, &y);
        if (run == 0) {
            baseX = x;
            baseY = y;
        }
        else if (x != baseX || y != baseY)
            mismatches++;
    }

    printf("Deterministic final position: (%d, %d)\n", baseX, baseY);
    r |= assertEquals(mismatches, 0);

    return r;
}
