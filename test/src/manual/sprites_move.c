#include <cmdfx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../test.h"

int main() {
    Canvas_clearScreen();
    Canvas_hideCursor();

    char** data = malloc(sizeof(char*) * 5);
    data[0] = strdup("###");
    data[1] = strdup("# #");
    data[2] = strdup("# #");
    data[3] = strdup("###");
    data[4] = 0;

    CmdFX_Sprite* sprite = Sprite_create(data, 0, 0);

    Canvas_setCursor(2, Canvas_getHeight() - 3);
    printf("Dimensions: %dx%d\n", sprite->width, sprite->height);

    Sprite_draw(4, 4, sprite);

    for (int i = 0; i < 250; i++) {
        if (sprite->x >= Canvas_getWidth() - sprite->width) {
            Sprite_moveTo(sprite, 1, sprite->y);
            continue;
        }

        Sprite_moveBy(sprite, 1, 0);
        sleepMillis(30);
    }

    Canvas_showCursor();
    Sprite_free(sprite);
    return 0;
}