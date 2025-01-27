#include <stdio.h>

#include "cmdfx/canvas.h"
#include "cmdfx/animation/canvas.h"
#include "../test.h"

int main() {
    Canvas_clearScreen();

    int x = Canvas_getWidth() / 2;
    int y = Canvas_getHeight() / 2;

    Canvas_setForeground(0x00FF00);

    Canvas_hLine_anim(x - 10, y, 20, '$', 2.0);
    Canvas_vLine_anim(x, y - 5, 10, '$', 2.0);
    Canvas_clearScreen();

    Canvas_rect_anim(x - 10, y - 5, 20, 10, '$', 4.0);
    
    Canvas_setForeground(0x0000FF);
    Canvas_fillRect_anim(x - 10, y - 5, 20, 10, '$', 4.0);
    Canvas_clearScreen();

    Canvas_setForeground(0xFFD700);
    Canvas_line_anim(x - 20, y - 5, x + 20, y + 5, '#', 4.0);

    Canvas_setForeground(0xFFA700);
    Canvas_line_anim_reverse(x + 20, y - 5, x - 20, y + 5, '#', 4.0);

    Canvas_clearScreen();

    Canvas_resetFormat();

    Canvas_drawText_anim(x - 6, y, "Hello, World!", 3.0);
    Canvas_drawText_anim_reverse(x - 6, y + 1, "Bye, World!", 3.0);
    Canvas_clearScreen();

    Canvas_drawAsciiText_anim(x - 3, y, '%', "ABCDEF", 3.0);
    Canvas_clearScreen();

    return 0;
}