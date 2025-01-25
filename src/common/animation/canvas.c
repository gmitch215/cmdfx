#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cmdfx/canvas.h"
#include "cmdfx/util.h"

// Utility Functions - Shapes

void Canvas_hLine_anim(int x, int y, int width, char c, double time) {
    if (x < 1 || y < 1) return;
    if (width < 1) return;
    if (time <= 0) return;

    double step = time / width;

    for (int i = 0; i < width; i++) {
        Canvas_setCursor(x + i, y);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));
    }
}

void Canvas_vLine_anim(int x, int y, int height, char c, double time) {
    if (x < 1 || y < 1) return;
    if (height < 1) return;
    if (time <= 0) return;

    double step = time / height;

    for (int i = 0; i < height; i++) {
        Canvas_setCursor(x, y + i);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));
    }
}

void Canvas_rect_anim(int x, int y, int width, int height, char c, double time) {
    if (x < 1 || y < 1) return;
    if (width < 1 || height < 1) return;
    if (time <= 0) return;

    double time0 = time / 4;

    Canvas_hLine_anim(x, y, width, c, time0);
    Canvas_vLine_anim(x, y, height, c, time0);
    Canvas_vLine_anim(x + width - 1, y, height, c, time0);
    Canvas_hLine_anim(x, y + height - 1, width, c, time0);
}

void Canvas_fillRect_anim(int x, int y, int width, int height, char c, double time) {
    if (x < 1 || y < 1) return;
    if (width < 1 || height < 1) return;
    if (time <= 0) return;

    double time0 = time / height;

    for (int i = 0; i < height; i++) {
        Canvas_hLine_anim(x, y + i, width, c, time0);
    }
}

void Canvas_circle_anim(int x, int y, int radius, char c, double time) {
    if (x < 1 || y < 1) return;
    if (radius < 1) return;
    if (time <= 0) return;

    double step = time / (2 * radius);

    for (int i = 0; i <= radius; i++) {
        int y0 = (int) (radius - i);
        int y1 = (int) (radius + i);

        Canvas_setCursor(x + radius, y + y0);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));

        Canvas_setCursor(x + radius, y + y1);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));
    }
}

void Canvas_fillCircle_anim(int x, int y, int radius, char c, double time) {
    if (x < 1 || y < 1) return;
    if (radius < 1) return;
    if (time <= 0) return;

    double step = time / (2 * radius);

    for (int i = 0; i <= radius; i++) {
        int y0 = (int) (radius - i);
        int y1 = (int) (radius + i);

        Canvas_hLine_anim(x + radius - i, y + y0, 2 * i + 1, c, step);
        Canvas_hLine_anim(x + radius - i, y + y1, 2 * i + 1, c, step);
    }
}

void Canvas_ellipse_anim(int x, int y, int xradius, int yradius, char c, double time) {
    if (x < 1 || y < 1) return;
    if (xradius < 1 || yradius < 1) return;
    if (time <= 0) return;

    double step = time / (2 * xradius);

    for (int i = 0; i <= xradius; i++) {
        int y0 = (int) (yradius * sqrt(1 - (i * i) / (xradius * xradius)));
        int y1 = (int) (2 * yradius - y0);

        Canvas_setCursor(x + xradius + i, y + y0);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));

        Canvas_setCursor(x + xradius + i, y + y1);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));

        Canvas_setCursor(x + xradius - i, y + y0);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));

        Canvas_setCursor(x + xradius - i, y + y1);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));
    }
}

void Canvas_fillEllipse_anim(int x, int y, int xradius, int yradius, char c, double time) {
    if (x < 1 || y < 1) return;
    if (xradius < 1 || yradius < 1) return;
    if (time <= 0) return;

    double step = time / (2 * xradius);

    for (int i = 0; i <= xradius; i++) {
        int y0 = (int) (yradius * sqrt(1 - (i * i) / (xradius * xradius)));
        int y1 = (int) (2 * yradius - y0);

        Canvas_hLine_anim(x + xradius - i, y + y0, 2 * i + 1, c, step);
        Canvas_hLine_anim(x + xradius - i, y + y1, 2 * i + 1, c, step);
    }
}

void Canvas_line_anim(int x0, int y0, int x1, int y1, char c, double time) {
    if (x0 < 1 || y0 < 1 || x1 < 1 || y1 < 1) return;
    if (time <= 0) return;

    double dx = x1 - x0;
    double dy = y1 - y0;
    double dist = sqrt(dx * dx + dy * dy);
    double step = time / dist;

    for (double i = 0; i <= dist; i++) {
        double x = x0 + (i * dx) / dist;
        double y = y0 + (i * dy) / dist;

        Canvas_setCursor((int) x, (int) y);
        putchar(c);
        sleepMillis((unsigned long) (step * 1000));
    }
}

// Utility Functions - Text

void Canvas_drawText_anim(int x, int y, const char* text, double time) {
    if (x < 1 || y < 1) return;
    if (time <= 0) return;

    int len = (int) strlen(text);
    double step = time / len;

    for (int i = 0; i < len; i++) {
        Canvas_setCursor(x + i, y);
        putchar(text[i]);
        sleepMillis((unsigned long) (step * 1000));
    }
}

void Canvas_drawAsciiText_anim(int x, int y, char character, const char* text, double time) {
    if (x < 1 || y < 1) return;
    if (time <= 0) return;
    if (text == 0) return;

    int len = (int) strlen(text);
    double step = time / len;

    for (int i = 0; i < len; i++) {
        char ch[2] = { text[i], '\0' };
        Canvas_drawAsciiText(x + i, y, character, ch);
        sleepMillis((unsigned long) (step * 1000));
    }
}