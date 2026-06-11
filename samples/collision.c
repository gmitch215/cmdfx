/**
 * @file collision.c
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @date 2025-04-03
 *
 * This is a simple demo program showcasing the collision detection and
 * resolution in the CmdFX Physics Engine.
 *
 * Two blocks of different mass slide toward each other across a frictionless
 * floor and bounce apart according to an elastic collision. The optional first
 * argument scales the simulation speed.
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cmdfx.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    double speed = 1;
    if (argc > 1) {
        double s = atof(argv[1]);
        if (s > 0) speed = s;
    }

    CmdFX_initThreadSafe();

    Canvas_clearScreen();
    Canvas_hideCursor();

    int width = Canvas_getWidth();
    int height = Canvas_getHeight();
    if (width < 20 || height < 12) {
        Canvas_showCursor();
        printf("Terminal too small for the collision demo.\n");
        return 1;
    }

    // pure horizontal collision: no gravity and no friction so the blocks
    // coast toward each other and bounce instead of accelerating off-screen
    Engine_setForceOfGravity(0);
    Engine_setDefaultFrictionCoefficient(0);
    CmdFX_setTickSpeed((int) (CmdFX_getTickSpeed() * speed));

    // two equal-size blocks with different masses
    CmdFX_Sprite* sprite1 =
        Sprite_create(Char2DBuilder_createFilled(5, 5, '#'), 0, 0);
    CmdFX_Sprite* sprite2 =
        Sprite_create(Char2DBuilder_createFilled(5, 5, '@'), 0, 0);
    Engine_setCharacterMass('#', 1.0);
    Engine_setCharacterMass('@', 1.5);

    // straddle the vertical middle and rest on a cosmetic floor line
    int row = height / 2 - 2;
    Canvas_hLine(1, row + 5, width, '-');

    Sprite_draw(3, row, sprite1);
    Sprite_draw(width - 5 - 2, row, sprite2);

    Sprite_setForegroundAll(sprite1, 0x00BFFF);
    Sprite_setForegroundAll(sprite2, 0xFF5555);

    // gentle approaching velocities; small per-tick steps keep the collision
    // visible instead of tunneling through each other
    Sprite_setVelocityX(sprite1, 2.0);
    Sprite_setVelocityX(sprite2, -2.0);

    Engine_enableMotionDebug();
    Engine_start();

    sleepMillis((unsigned long) (8000 / speed));

    Engine_end();
    Sprite_free(sprite1);
    Sprite_free(sprite2);
    CmdFX_destroyThreadSafe();
    Canvas_showCursor();

    return 0;
}
