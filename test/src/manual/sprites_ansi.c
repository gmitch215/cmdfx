#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmdfx.h>

#include "../test.h"

int main() {
    Canvas_clearScreen();
    Canvas_hideCursor();

    CmdFX_Sprite* sprite1 = Sprite_createFilled(5, 5, '#', 0, 0);
    Sprite_setForegroundAll(sprite1, 0xFF0000);
    Sprite_setBackgroundAll(sprite1, 0x00FF00);
    Sprite_draw(5, 5, sprite1);

    // FIXME: Gradients do not work
    CmdFX_Sprite* sprite2 = Sprite_createFilled(5, 5, '#', 0, 0);
    Sprite_setForegroundGradientAll(sprite2, GRADIENT_HORIZONTAL, 2, 0xFFD700, 0x00FF00);
    Sprite_draw(15, 5, sprite2);

    Canvas_showCursor();

    sleepMillis(5000);

    Sprite_free(sprite1);
    Sprite_free(sprite2);

    return 0;
}