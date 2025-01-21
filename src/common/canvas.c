#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "cmdfx/canvas.h"

// Core Functions

void Canvas_setChar(int x, int y, char c) {
    if (x < 1) return;
    if (y < 1) return;

    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    Canvas_setCursor(x, y);
    putchar(c);
    Canvas_setCursor(oldX, oldY);
}

void Canvas_setAnsiCurrent(const char* ansi) {
    char* format = malloc(5 + strlen(ansi) + 1);
    sprintf(format, "\033[%sm", ansi);

    printf("%s", format);
    free(format);
}

void Canvas_setAnsi(int x, int y, const char* ansi) {
    if (x < 1) return;
    if (y < 1) return;

    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    Canvas_setCursor(x, y);
    Canvas_setAnsiCurrent(ansi);
    Canvas_setCursor(oldX, oldY);
}

// Utility Functions

void Canvas_resetFormat() {
    printf("\033[0m");
}

void Canvas_setForeground(int rgb) {
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    char* ansi = malloc(17);
    sprintf(ansi, "38;2;%d;%d;%d", r, g, b);

    Canvas_setAnsiCurrent(ansi);
    free(ansi);
}

void Canvas_setBackground(int rgb) {
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    char* ansi = malloc(17);
    sprintf(ansi, "48;2;%d;%d;%d", r, g, b);

    Canvas_setAnsiCurrent(ansi);
    free(ansi);
}

// Utility Functions - Shapes

void Canvas_hLine(int x, int y, int width, char c) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    for (int i = 0; i < width; i++) {
        Canvas_setCursor(x + i, y);
        putchar(c);
    }

    Canvas_setCursor(oldX, oldY);
}

void Canvas_vLine(int x, int y, int height, char c) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    for (int i = 0; i < height; i++) {
        Canvas_setCursor(x, y + i);
        putchar(c);
    }

    Canvas_setCursor(oldX, oldY);
}

void Canvas_rect(int x, int y, int width, int height, char c) {
    Canvas_hLine(x, y, width, c);
    Canvas_hLine(x, y + height - 1, width, c);
    Canvas_vLine(x, y, height, c);
    Canvas_vLine(x + width - 1, y, height, c);
}

void Canvas_fillRect(int x, int y, int width, int height, char c) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    for (int i = 0; i < height; i++) {
        Canvas_hLine(x, y + i, width, c);
    }

    Canvas_setCursor(oldX, oldY);
}

void Canvas_circle(int x, int y, int radius, char c) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    int x1 = 0;
    int y1 = radius;
    int d = 1 - radius;

    while (x1 <= y1) {
        Canvas_setChar(x + x1, y + y1, c);
        Canvas_setChar(x - x1, y + y1, c);
        Canvas_setChar(x + x1, y - y1, c);
        Canvas_setChar(x - x1, y - y1, c);
        Canvas_setChar(x + y1, y + x1, c);
        Canvas_setChar(x - y1, y + x1, c);
        Canvas_setChar(x + y1, y - x1, c);
        Canvas_setChar(x - y1, y - x1, c);

        if (d < 0) {
            d += 2 * x1 + 3;
        } else {
            d += 2 * (x1 - y1) + 5;
            y1--;
        }
        x1++;
    }

    Canvas_setCursor(oldX, oldY);
}

void Canvas_fillCircle(int x, int y, int radius, char c) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    int radiusSq = radius * radius;

    for (int y1 = -radius; y1 <= radius; y1++) {
        int y1Sq = y1 * y1;
        int x1 = 0;
        while (x1 * x1 + y1Sq <= radiusSq) x1++;
        x1--;

        for (int x2 = -x1; x2 <= x1; x2++) {
            Canvas_setChar(x + x2, y + y1, c);
        }
    }

    Canvas_setCursor(oldX, oldY);
}

void Canvas_ellipse(int x, int y, int xradius, int yradius, char c) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    int x1 = 0;
    int y1 = yradius;
    int xradiusSq = xradius * xradius;
    int yradiusSq = yradius * yradius;
    int twoXradiusSq = 2 * xradiusSq;
    int twoYradiusSq = 2 * yradiusSq;
    int p;
    int px = 0;
    int py = twoXradiusSq * y1;

    p = yradiusSq - (xradiusSq * yradius) + (0.25 * xradiusSq);
    while (px < py) {
        Canvas_setChar(x + x1, y + y1, c);
        Canvas_setChar(x - x1, y + y1, c);
        Canvas_setChar(x + x1, y - y1, c);
        Canvas_setChar(x - x1, y - y1, c);

        x1++;
        px += twoYradiusSq;

        if (p < 0) {
            p += yradiusSq + px;
        } else {
            y1--;
            py -= twoXradiusSq;
            p += yradiusSq + px - py;
        }
    }

    p = yradiusSq * (x1 + 0.5) * (x1 + 0.5) + xradiusSq * (y1 - 1) * (y1 - 1) - xradiusSq * yradiusSq;
    while (y1 >= 0) {
        Canvas_setChar(x + x1, y + y1, c);
        Canvas_setChar(x - x1, y + y1, c);
        Canvas_setChar(x + x1, y - y1, c);
        Canvas_setChar(x - x1, y - y1, c);

        y1--;
        py -= twoXradiusSq;

        if (p > 0) {
            p += xradiusSq - py;
        } else {
            x1++;
            px += twoYradiusSq;
            p += xradiusSq - py + px;
        }
    }

    Canvas_setCursor(oldX, oldY);
}

void Canvas_fillEllipse(int x, int y, int xradius, int yradius, char c) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    for (int dx = -xradius; dx <= xradius; dx++) {
        for (int dy = -yradius; dy <= yradius; dy++) {
            double ellipseEquation = 
                (dx * dx) / (double)(xradius * xradius) + 
                (dy * dy) / (double)(yradius * yradius);

            if (ellipseEquation <= 1.0) {
                int px = x + dx;
                int py = y + dy;

                Canvas_setChar(px, py, c);
            }
        }
    }

    Canvas_setCursor(oldX, oldY);
}
