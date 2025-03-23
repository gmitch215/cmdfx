/**
 * @file square-adventure.c
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @date 2025-03-22
 * 
 * This is a simple game where you control a square with WASD and change its color with the number keys.
 * 
 * Possible Color Changes:
 * - 1: White (default)
 * - 2: Red
 * - 3: Green
 * - 4: Blue
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <cmdfx.h>
#include <stdio.h>

CmdFX_Sprite* square = 0;
int width = -1;
int height = -1;

int running = 1;

int onKeyPress(CmdFX_Event* event) {
    CmdFX_KeyEvent* payload = (CmdFX_KeyEvent*) event->data;
    if (square == 0) return 0;

    if (square->x < 0) return 0;
    if (square->x + square->width >= width) return 0;
    if (square->y < 0) return 0;
    if (square->y + square->height >= height) return 0;

    char c = payload->keyChar;
    if (c == 'w') {
        Sprite_moveBy(square, 0, -1);
    } else if (c == 'a') {
        Sprite_moveBy(square, -1, 0);
    } else if (c == 's') {
        Sprite_moveBy(square, 0, 1);
    } else if (c == 'd') {
        Sprite_moveBy(square, 1, 0);
    }

    if (c == 'q') {
        Canvas_showCursor();
        Screen_setEchoEnabled(1);
        Screen_setLineBuffered(1);
        running = 0;
    }

    switch (c) {
        case '0':
            Sprite_setForegroundAll(square, 0xFFFFFF);
            break;
        case '1':
            Sprite_setForegroundAll(square, 0xFF0000);
            break;
        case '2':
            Sprite_setForegroundAll(square, 0x00FF00);
            break;
        case '3':
            Sprite_setForegroundAll(square, 0x0000FF);
            break;
        case '4':
            Sprite_setForegroundAll(square, 0xDB09F7);
            break;
        case '5':
            Sprite_setForegroundAll(square, 0x09EBF7);
            break;
        case '6':
            Sprite_setForegroundAll(square, 0xFFD700);
            break;
        case '7':
            Sprite_setForegroundAll(square, 0xFFA700);
            break;
        case '8':
            Sprite_setForegroundAll(square, 0x9C09F7);
            break;
        case '9':
            Sprite_setForegroundAll(square, 0x222222);
            break;
    }

    return 0;
}

int main() {
    Canvas_clearScreen();
    Canvas_hideCursor();
    Screen_setEchoEnabled(0);
    Screen_setLineBuffered(0);

    width = Canvas_getWidth();
    height = Canvas_getHeight();

    square = Sprite_create(
        Char2DBuilder_createFilled(3, 3, '#'),
        String2DBuilder_create(3, 3),
        0
    );
    Sprite_draw(width / 2, height / 2, square);
    addCmdFXEventListener(CMDFX_EVENT_KEY, onKeyPress);

    printf("Use WASD to move the square and the number keys to change its color.\nPress 'Q' to quit.\n");

    while (running) {}

    shutdownCmdFXEvents();
    return 0;
}