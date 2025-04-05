/**
 * @file collision.c
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @date 2025-04-03
 * 
 * This is a simple demo program showcasing the collision detection and resolution
 * in the CmdFX Physics Engine.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <cmdfx.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char** data1 = Char2DBuilder_createFilled(5, 5, '#');
    CmdFX_Sprite* sprite1 = Sprite_create(data1, 0, 0);
    Engine_setCharacterMass('#', 1.0);

    char** data2 = Char2DBuilder_createFilled(5, 5, '@');
    CmdFX_Sprite* sprite2 = Sprite_create(data2, 0, 0);
    Engine_setCharacterMass('@', 1.5);

    double speed = 1;
    if (argc > 1) {
        speed = atof(argv[1]);
        if (speed > 0) CmdFX_setTickSpeed((int) (CmdFX_getTickSpeed() * speed));
    }
    double friction = Engine_getDefaultFrictionCoefficient();
    if (argc > 2) {
        friction = atof(argv[2]);
        if (friction >= 0) Engine_setDefaultFrictionCoefficient(friction);
    }

    Canvas_clearScreen();
    Canvas_hideCursor();
    Engine_enableMotionDebug();
    CmdFX_initThreadSafe();

    Canvas_hLine(0, Engine_getGroundY(), Canvas_getWidth(), '-');
    Canvas_hLine(0, 1, Canvas_getWidth(), '-');
    
    Sprite_draw(2, Canvas_getHeight() - sprite1->height - 1, sprite1);
    Sprite_draw(Canvas_getWidth() - sprite2->width - 2, Canvas_getHeight() - sprite2->height - 1, sprite2);
    
    Engine_start();

    CmdFX_Vector* velocity1 = Vector_create(3.0, 0);
    CmdFX_Vector* velocity2 = Vector_create(-2.5, 0);

    Sprite_addForceFor(sprite1, velocity1, 300 / speed);
    Sprite_addForceFor(sprite2, velocity2, 300 / speed);
    sleepMillis(10000 / speed);

    Engine_end();
    Sprite_free(sprite1);
    Sprite_free(sprite2);

    CmdFX_destroyThreadSafe();
    Canvas_showCursor();

    return 0;
}