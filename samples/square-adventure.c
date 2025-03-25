/**
 * @file square-adventure.c
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @date 2025-03-22
 * 
 * This is a simple game where you control a square with WASD and change its color with the number keys.
 * 
 * Possible Color Changes:
 * - 0: White (default)
 * - 1: Red
 * - 2: Green
 * - 3: Blue
 * - 4: Pink
 * - 5: Cyan
 * - 6: Gold
 * - 7: Orange
 * - 8: Purple
 * - 9: Black
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <cmdfx.h>
#include <stdio.h>

CmdFX_Sprite* square = 0;

int running = 1;

int onKeyPress(CmdFX_Event* event) {
    CmdFX_KeyEvent* payload = (CmdFX_KeyEvent*) event->data;
    if (square == 0) return 0;

    char c = payload->keyChar; // Get character pressed in key event
    if (c == 'w') {
        Sprite_moveBy(square, 0, -1);
    } else if (c == 'a') {
        Sprite_moveBy(square, -1, 0);
    } else if (c == 's') {
        Sprite_moveBy(square, 0, 1);
    } else if (c == 'd') {
        Sprite_moveBy(square, 1, 0);
    }
    if (c == 'q') { // When 'Q' is pressed, turn off the application
        Canvas_showCursor();
        Screen_setEchoEnabled(1);
        Screen_setLineBuffered(1);
        running = 0;
    }

    // Change Foreground Color based on number key pressed
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
    // Main Sample Entrypoint

    // Start with clearing the screen
    Canvas_clearScreen();

    // Then, hide the cursor
    Canvas_hideCursor();

    // Turn off echo and line buffering so we can handle input without
    // typing anything on the screen
    Screen_setEchoEnabled(0);
    Screen_setLineBuffered(0);

    // Get terminal bounds
    int width = Canvas_getWidth();
    int height = Canvas_getHeight();

    // Initialize global sprite to be a 3x3 square of the '#' character
    square = Sprite_create(
        Char2DBuilder_createFilled(3, 3, '#'),
        String2DBuilder_create(3, 3), // Initialize ANSI codes to be empty (specify no color)
        0
    );
    Sprite_draw(width / 2, height / 2, square); // Draw the square in the middle of the screen
    addCmdFXEventListener(CMDFX_EVENT_KEY, onKeyPress); // Register the key event listener

    printf("Use WASD to move the square and the number keys to change its color.\nPress 'Q' to quit.\n");

    while (running) {} // Hang the application until 'Q' is pressed

    shutdownCmdFXEvents(); // Once the application is done, free the event listeners
    return 0;
}