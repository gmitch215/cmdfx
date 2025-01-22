#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmdfx.h>

#include "../test.h"

CmdFX_Sprite* sprite;

void printDimensions() {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    Canvas_setCursor(2, Canvas_getHeight() - 3);
    printf("Dimensions: %dx%d\n", sprite->width, sprite->height);
    Canvas_setCursor(oldX, oldY);
}

int main() {
    Canvas_clearScreen();
    Canvas_hideCursor();

    int x = Canvas_getWidth() / 2;
    int y = Canvas_getHeight() / 2;

    sprite = Sprite_create(0, 0, 0);
    printDimensions();
    sleepMillis(3000);

    Sprite_resize(sprite, 5, 5);
    Sprite_setChar(sprite, 0, 0, '#');
    Sprite_setChar(sprite, 1, 1, '#');
    Sprite_setChar(sprite, 2, 2, '#');
    Sprite_setChar(sprite, 3, 3, '#');
    Sprite_setChar(sprite, 4, 4, '#');

    printDimensions();

    Sprite_draw(x, y, sprite);
    sleepMillis(3000);

    Sprite_resize(sprite, 3, 3);
    printDimensions();
    sleepMillis(3000);

    Sprite_resizeWithPadding(sprite, 7, 7, '%');
    printDimensions();
    sleepMillis(3000);

    Sprite_resizeAndCenter(sprite, 9, 9);
    printDimensions();
    sleepMillis(3000);

    Canvas_showCursor();
    Sprite_free(sprite);
    Canvas_clearScreen();
    return 0;
}