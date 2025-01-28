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
    Sprite_draw(3, 3, sprite1);

    CmdFX_Sprite* sprite2 = Sprite_createFilled(5, 5, '#', 0, 0);
    Sprite_setForegroundGradientAll(sprite2, GRADIENT_HORIZONTAL, 2, 0xFFD700, 0x00FF00);
    Sprite_draw(10, 3, sprite2);

    CmdFX_Sprite* sprite3 = Sprite_createFilled(5, 5, '#', 0, 0);
    Sprite_setForegroundGradientAll(sprite3, GRADIENT_VERTICAL, 3, 0xFFA700, 0x0000FF, 0x00FF00);
    Sprite_draw(17, 3, sprite3);

    CmdFX_Sprite* sprite4 = Sprite_createFilled(7, 7, '#', 0, 0);
    Sprite_setForegroundGradientAll(sprite4, GRADIENT_ANGLE_45, 4, 0xFF0000, 0xAABBCC, 0x00FD70, 0x00FF00);
    Sprite_draw(25, 3, sprite4);

    CmdFX_Sprite* sprite5 = Sprite_createFilled(7, 7, '#', 0, 0);
    Sprite_setForegroundGradientAll(sprite5, GRADIENT_ANGLE_135, 4, 0xFF0000, 0xAABBCC, 0x00FD70, 0x00FF00);
    Sprite_draw(33, 3, sprite5);

    CmdFX_Sprite* sprite6 = Sprite_createFilled(7, 7, '#', 0, 0);
    Sprite_setForegroundGradientAll(sprite6, GRADIENT_RADIAL, 2, 0xFFFF00, 0x00FFFF);
    Sprite_draw(41, 3, sprite6);

    CmdFX_Sprite* sprite7 = Sprite_createFilled(8, 8, '#', 0, 0);
    Sprite_setForegroundGradient(sprite7, 0, 0, sprite7->width, sprite7->height / 2, GRADIENT_CONICAL, 2, 0xFFD700, 0x0000FF);
    Sprite_setBackgroundGradient(sprite7, 0, sprite7->height / 2, sprite7->width, sprite7->height / 2, GRADIENT_CONICAL, 2, 0xFFD700, 0x0000FF);
    Sprite_draw(49, 3, sprite7);

    Canvas_showCursor();

    sleepMillis(5000);

    Sprite_free(sprite1);
    Sprite_free(sprite2);
    Sprite_free(sprite3);
    Sprite_free(sprite4);
    Sprite_free(sprite5);
    Sprite_free(sprite6);
    Sprite_free(sprite7);

    return 0;
}