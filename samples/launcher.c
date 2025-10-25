/**
 * @file launcher.c
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @date 2025-03-28
 *
 * This is a simple demo program that shows how to use the Physics Engine for
 * CmdFX.
 *
 * Simply run the program and watch the object fly around the screen.
 * You can optionally pass a multiplicative modifier to the tick speed as the
 * first parameter to see how fast it will go.
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cmdfx.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char** data = Char2DBuilder_createFilled(5, 5, '%');
    data[0][0] = 0;
    data[4][0] = 0;
    data[0][4] = 0;
    data[4][4] = 0;

    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);

    double speed = 1;
    if (argc > 1) {
        speed = atof(argv[1]);
        if (speed > 0) CmdFX_setTickSpeed((int) (CmdFX_getTickSpeed() * speed));
    }

    if (argc > 2) {
        double friction = atof(argv[2]);
        if (friction >= 0) Engine_setDefaultFrictionCoefficient(atof(argv[2]));
    }

    // Initialize threading first, before any drawing operations
    CmdFX_initThreadSafe();

    Canvas_clearScreen();
    Canvas_hideCursor();
    Engine_enableMotionDebug();

    // Draw static elements first
    Canvas_hLine(0, Engine_getGroundY(), Canvas_getWidth(), '-');
    Canvas_hLine(0, 1, Canvas_getWidth(), '-');
    Sprite_draw(2, Canvas_getHeight() - sprite->height - 1, sprite);

    // Start physics engine after all initial drawing is done
    Engine_start();

    CmdFX_Vector* velocity = Vector_create(2, 2);
    Sprite_addForceFor(sprite, velocity, 300 / speed);
    sleepMillis(10000 / speed);

    // Cleanup in proper order: engine first, then threading, then sprites
    Engine_end();
    free(velocity);
    Sprite_free(sprite);
    CmdFX_destroyThreadSafe();

    Canvas_showCursor();

    return 0;
}