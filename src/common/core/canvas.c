#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "cmdfx/core/canvas.h"

// Core Functions

void Canvas_setChar(int x, int y, char c) {
    if (x < 0) return;
    if (y < 0) return;

    Canvas_setCursor(x, y);
    putchar(c);
}

void Canvas_setAnsiCurrent(const char* ansi) {
    printf("%s", ansi);
}

void Canvas_setAnsi(int x, int y, const char* ansi) {
    if (x < 0) return;
    if (y < 0) return;

    Canvas_setCursor(x, y);
    Canvas_setAnsiCurrent(ansi);
}

// Utility Functions

void Canvas_resetFormat() {
    printf("\033[0m");
}

void Canvas_setForeground(int rgb) {
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    char* ansi = malloc(22);
    snprintf(ansi, 22, "\033[38;2;%d;%d;%dm", r, g, b);

    Canvas_setAnsiCurrent(ansi);
    free(ansi);
}

void Canvas_setBackground(int rgb) {
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    char* ansi = malloc(22);
    snprintf(ansi, 22, "\033[48;2;%d;%d;%dm", r, g, b);

    Canvas_setAnsiCurrent(ansi);
    free(ansi);
}

void Canvas_setColor8(int color) {
    if (color < 30 || color > 107) return;

    char* ansi = malloc(9);
    snprintf(ansi, 9, "\033[%dm", color);

    Canvas_setAnsiCurrent(ansi);
    free(ansi);
}

void Canvas_setForeground256(int color) {
    if (color < 0 || color > 255) return;

    char* ansi = malloc(14);
    snprintf(ansi, 14, "\033[38;5;%dm", color);

    Canvas_setAnsiCurrent(ansi);
    free(ansi);
}

void Canvas_setBackground256(int color) {
    if (color < 0 || color > 255) return;

    char* ansi = malloc(14);
    snprintf(ansi, 14, "\033[48;5;%dm", color);

    Canvas_setAnsiCurrent(ansi);
    free(ansi);
}

void Canvas_enableBold() {
    printf("\033[1m");
}

void Canvas_disableBold() {
    printf("\033[22m");
}

void Canvas_enableDim() {
    printf("\033[2m");
}

void Canvas_disableDim() {
    printf("\033[22m");
}

void Canvas_enableItalic() {
    printf("\033[3m");
}

void Canvas_disableItalic() {
    printf("\033[23m");
}

void Canvas_enableUnderline() {
    printf("\033[4m");
}

void Canvas_disableUnderline() {
    printf("\033[24m");
}

void Canvas_enableBlink() {
    printf("\033[5m");
}

void Canvas_disableBlink() {
    printf("\033[25m");
}

void Canvas_enableInvert() {
    printf("\033[7m");
}

void Canvas_disableInvert() {
    printf("\033[27m");
}

void Canvas_enableHidden() {
    printf("\033[8m");
}

void Canvas_disableHidden() {
    printf("\033[28m");
}

void Canvas_enableStrikethrough() {
    printf("\033[9m");
}

void Canvas_disableStrikethrough() {
    printf("\033[29m");
}

// Utility Functions - Shapes

void Canvas_hLine(int x, int y, int width, char c) {
    for (int i = 0; i < width; i++) {
        Canvas_setCursor(x + i, y);
        putchar(c);
    }
}

void Canvas_vLine(int x, int y, int height, char c) {
    for (int i = 0; i < height; i++) {
        Canvas_setCursor(x, y + i);
        putchar(c);
    }
}

void Canvas_rect(int x, int y, int width, int height, char c) {
    Canvas_hLine(x, y, width, c);
    Canvas_hLine(x, y + height - 1, width, c);
    Canvas_vLine(x, y, height, c);
    Canvas_vLine(x + width - 1, y, height, c);
}

void Canvas_fillRect(int x, int y, int width, int height, char c) {
    for (int i = 0; i < height; i++) {
        Canvas_hLine(x, y + i, width, c);
    }
}

void Canvas_circle(int x, int y, int radius, char c) {
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
}

void Canvas_fillCircle(int x, int y, int radius, char c) {
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
}

void Canvas_ellipse(int x, int y, int xradius, int yradius, char c) {
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
}

void Canvas_fillEllipse(int x, int y, int xradius, int yradius, char c) {
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
}

void Canvas_line(int x1, int y1, int x2, int y2, char c) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        Canvas_setChar(x1, y1, c);
        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void Canvas_polygon(int x, int y, int sides, int radius, char c) {
    if (x < 0 || y < 0) return;
    if (radius < 1) return;
    if (sides < 3) return;

    double angleStep = 2 * M_PI / sides;
    
    int prevX = x + radius; // * cos(0);
    int prevY = y; // + (radius * sin(0));

    for (int i = 1; i <= sides; i++) {
        int nextX = x + radius * cos(i * angleStep);
        int nextY = y + radius * sin(i * angleStep);

        Canvas_line(prevX, prevY, nextX, nextY, c);

        prevX = nextX;
        prevY = nextY;
    }
}

void Canvas_fillPolygon(int x, int y, int sides, int radius, char c) {
    if (x < 0 || y < 0) return;
    if (radius < 1) return;
    if (sides < 3) return;

    double angleStep = 2 * M_PI / sides;
    int *vx = malloc(sides * sizeof(int));
    int *vy = malloc(sides * sizeof(int));

    if (!vx || !vy) return;

    for (int i = 0; i < sides; i++) {
        vx[i] = x + radius * cos(i * angleStep);
        vy[i] = y + radius * sin(i * angleStep);
    }

    int minY = vy[0], maxY = vy[0];
    for (int i = 1; i < sides; i++) {
        if (vy[i] < minY) minY = vy[i];
        if (vy[i] > maxY) maxY = vy[i];
    }

    for (int scanY = minY; scanY <= maxY; scanY++) {
        int *intersections = calloc(sides, sizeof(int));
        int count = 0;

        for (int i = 0; i < sides; i++) {
            int j = (i + 1) % sides;
            int y1 = vy[i], y2 = vy[j], x1 = vx[i], x2 = vx[j];

            if ((y1 <= scanY && y2 > scanY) || (y2 <= scanY && y1 > scanY)) {
                double t = (double) (scanY - y1) / (y2 - y1);
                intersections[count++] = x1 + t * (x2 - x1);
            }
        }

        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (intersections[i] > intersections[j]) {
                    int temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }

        for (int i = 0; i < count; i += 2) {
            if (i + 1 < count) {
                for (int xFill = intersections[i]; xFill <= intersections[i + 1]; xFill++) {
                    Canvas_setChar(xFill, scanY, c);
                }
            }
        }

        free(intersections);
    }

    free(vx);
    free(vy);
}

// Utility Functions - Text

void Canvas_drawText(int x, int y, char* text) {
    Canvas_setCursor(x, y);
    printf("%s", text);
}

void Canvas_drawAscii(int x, int y, char ascii[8][5]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            Canvas_setCursor(x + j, y + i);
            putchar(ascii[i][j]);
        }
    }
}

#pragma region ASCII Art

char ASCII_EMPTY[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_A[8][5] = {
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_A[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_B[8][5] = {
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_B[8][5] = {
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_C[8][5] = {
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_C[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_D[8][5] = {
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_D[8][5] = {
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_E[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_E[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_F[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_F[8][5] = {
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_G[8][5] = {
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_G[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '}
};

char ASCII_UPPER_H[8][5] = {
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_H[8][5] = {
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_I[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_I[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_J[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_J[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}
};

char ASCII_UPPER_K[8][5] = {
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_K[8][5] = {
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_L[8][5] = {
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_L[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_M[8][5] = {
    {'#', ' ', ' ', ' ', '#'},
    {'#', '#', ' ', '#', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_M[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_N[8][5] = {
    {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', '#', '#', ' ', '#'},
    {' ', '#', ' ', '#', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_N[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_O[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_O[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_P[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_P[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}
};

char ASCII_UPPER_Q[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_Q[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '}
};

char ASCII_UPPER_R[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_R[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_S[8][5] = {
    {' ', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_S[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_T[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_T[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_U[8][5] = {
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_U[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_V[8][5] = {
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_V[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_W[8][5] = {
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', '#', ' ', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_W[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', '#'},
    {'#', '#', ' ', '#', '#'},
    {'#', ' ', ' ', ' ', '#'},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_X[8][5] = {
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_X[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_Y[8][5] = {
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_Y[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}
};

char ASCII_UPPER_Z[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_Z[8][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_ZERO[8][5] = {
    {' ', '#', '#', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_ONE[8][5] = {
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_TWO[8][5] = {
    {' ', '#', '#', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_THREE[8][5] = {
    {' ', '#', '#', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_FOUR[8][5] = {
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {'#', '#', '#', '#', '#'},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_FIVE[8][5] = {
    {'#', '#', '#', '#', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_SIX[8][5] = {
    {' ', '#', '#', '#', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_SEVEN[8][5] = {
    {'#', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_EIGHT[8][5] = {
    {' ', '#', '#', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {'#', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_NINE[8][5] = {
    {' ', '#', '#', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {'#', ' ', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '},
    {'#', '#', '#', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

char ASCII_MAP[128][8][5];

#pragma endregion

void _initAsciiText() {
    // Set Everything to Empty
    for (int i = 0; i < 127; i++) {
        memcpy(ASCII_MAP[i], ASCII_EMPTY, sizeof(ASCII_EMPTY));
    }

    // Set Alphabet Characters
    memcpy(ASCII_MAP['A'], ASCII_UPPER_A, sizeof(ASCII_UPPER_A));
    memcpy(ASCII_MAP['a'], ASCII_LOWER_A, sizeof(ASCII_LOWER_A));
    memcpy(ASCII_MAP['B'], ASCII_UPPER_B, sizeof(ASCII_UPPER_B));
    memcpy(ASCII_MAP['b'], ASCII_LOWER_B, sizeof(ASCII_LOWER_B));
    memcpy(ASCII_MAP['C'], ASCII_UPPER_C, sizeof(ASCII_UPPER_C));
    memcpy(ASCII_MAP['c'], ASCII_LOWER_C, sizeof(ASCII_LOWER_C));
    memcpy(ASCII_MAP['D'], ASCII_UPPER_D, sizeof(ASCII_UPPER_D));
    memcpy(ASCII_MAP['d'], ASCII_LOWER_D, sizeof(ASCII_LOWER_D));
    memcpy(ASCII_MAP['E'], ASCII_UPPER_E, sizeof(ASCII_UPPER_E));
    memcpy(ASCII_MAP['e'], ASCII_LOWER_E, sizeof(ASCII_LOWER_E));
    memcpy(ASCII_MAP['F'], ASCII_UPPER_F, sizeof(ASCII_UPPER_F));
    memcpy(ASCII_MAP['f'], ASCII_LOWER_F, sizeof(ASCII_LOWER_F));
    memcpy(ASCII_MAP['G'], ASCII_UPPER_G, sizeof(ASCII_UPPER_G));
    memcpy(ASCII_MAP['g'], ASCII_LOWER_G, sizeof(ASCII_LOWER_G));
    memcpy(ASCII_MAP['H'], ASCII_UPPER_H, sizeof(ASCII_UPPER_H));
    memcpy(ASCII_MAP['h'], ASCII_LOWER_H, sizeof(ASCII_LOWER_H));
    memcpy(ASCII_MAP['I'], ASCII_UPPER_I, sizeof(ASCII_UPPER_I));
    memcpy(ASCII_MAP['i'], ASCII_LOWER_I, sizeof(ASCII_LOWER_I));
    memcpy(ASCII_MAP['J'], ASCII_UPPER_J, sizeof(ASCII_UPPER_J));
    memcpy(ASCII_MAP['j'], ASCII_LOWER_J, sizeof(ASCII_LOWER_J));
    memcpy(ASCII_MAP['K'], ASCII_UPPER_K, sizeof(ASCII_UPPER_K));
    memcpy(ASCII_MAP['k'], ASCII_LOWER_K, sizeof(ASCII_LOWER_K));
    memcpy(ASCII_MAP['L'], ASCII_UPPER_L, sizeof(ASCII_UPPER_L));
    memcpy(ASCII_MAP['l'], ASCII_LOWER_L, sizeof(ASCII_LOWER_L));
    memcpy(ASCII_MAP['M'], ASCII_UPPER_M, sizeof(ASCII_UPPER_M));
    memcpy(ASCII_MAP['m'], ASCII_LOWER_M, sizeof(ASCII_LOWER_M));
    memcpy(ASCII_MAP['N'], ASCII_UPPER_N, sizeof(ASCII_UPPER_N));
    memcpy(ASCII_MAP['n'], ASCII_LOWER_N, sizeof(ASCII_LOWER_N));
    memcpy(ASCII_MAP['O'], ASCII_UPPER_O, sizeof(ASCII_UPPER_O));
    memcpy(ASCII_MAP['o'], ASCII_LOWER_O, sizeof(ASCII_LOWER_O));
    memcpy(ASCII_MAP['P'], ASCII_UPPER_P, sizeof(ASCII_UPPER_P));
    memcpy(ASCII_MAP['p'], ASCII_LOWER_P, sizeof(ASCII_LOWER_P));
    memcpy(ASCII_MAP['Q'], ASCII_UPPER_Q, sizeof(ASCII_UPPER_Q));
    memcpy(ASCII_MAP['q'], ASCII_LOWER_Q, sizeof(ASCII_LOWER_Q));
    memcpy(ASCII_MAP['R'], ASCII_UPPER_R, sizeof(ASCII_UPPER_R));
    memcpy(ASCII_MAP['r'], ASCII_LOWER_R, sizeof(ASCII_LOWER_R));
    memcpy(ASCII_MAP['S'], ASCII_UPPER_S, sizeof(ASCII_UPPER_S));
    memcpy(ASCII_MAP['s'], ASCII_LOWER_S, sizeof(ASCII_LOWER_S));
    memcpy(ASCII_MAP['T'], ASCII_UPPER_T, sizeof(ASCII_UPPER_T));
    memcpy(ASCII_MAP['t'], ASCII_LOWER_T, sizeof(ASCII_LOWER_T));
    memcpy(ASCII_MAP['U'], ASCII_UPPER_U, sizeof(ASCII_UPPER_U));
    memcpy(ASCII_MAP['u'], ASCII_LOWER_U, sizeof(ASCII_LOWER_U));
    memcpy(ASCII_MAP['V'], ASCII_UPPER_V, sizeof(ASCII_UPPER_V));
    memcpy(ASCII_MAP['v'], ASCII_LOWER_V, sizeof(ASCII_LOWER_V));
    memcpy(ASCII_MAP['W'], ASCII_UPPER_W, sizeof(ASCII_UPPER_W));
    memcpy(ASCII_MAP['w'], ASCII_LOWER_W, sizeof(ASCII_LOWER_W));
    memcpy(ASCII_MAP['X'], ASCII_UPPER_X, sizeof(ASCII_UPPER_X));
    memcpy(ASCII_MAP['x'], ASCII_LOWER_X, sizeof(ASCII_LOWER_X));
    memcpy(ASCII_MAP['Y'], ASCII_UPPER_Y, sizeof(ASCII_UPPER_Y));
    memcpy(ASCII_MAP['y'], ASCII_LOWER_Y, sizeof(ASCII_LOWER_Y));
    memcpy(ASCII_MAP['Z'], ASCII_UPPER_Z, sizeof(ASCII_UPPER_Z));
    memcpy(ASCII_MAP['z'], ASCII_LOWER_Z, sizeof(ASCII_LOWER_Z));

    // Set Number Characters
    memcpy(ASCII_MAP['0'], ASCII_ZERO, sizeof(ASCII_ZERO));
    memcpy(ASCII_MAP['1'], ASCII_ONE, sizeof(ASCII_ONE));
    memcpy(ASCII_MAP['2'], ASCII_TWO, sizeof(ASCII_TWO));
    memcpy(ASCII_MAP['3'], ASCII_THREE, sizeof(ASCII_THREE));
    memcpy(ASCII_MAP['4'], ASCII_FOUR, sizeof(ASCII_FOUR));
    memcpy(ASCII_MAP['5'], ASCII_FIVE, sizeof(ASCII_FIVE));
    memcpy(ASCII_MAP['6'], ASCII_SIX, sizeof(ASCII_SIX));
    memcpy(ASCII_MAP['7'], ASCII_SEVEN, sizeof(ASCII_SEVEN));
    memcpy(ASCII_MAP['8'], ASCII_EIGHT, sizeof(ASCII_EIGHT));
    memcpy(ASCII_MAP['9'], ASCII_NINE, sizeof(ASCII_NINE));
}

void Canvas_drawAsciiText(int x, int y, char character, const char* text) {
    if (!text) return;
    if (strlen(text) == 0) return;

    _initAsciiText();

    for (int i = 0; i < strlen(text); i++) {
        char c = text[i];
        char ascii[8][5];

        if (c > 127) {
            memcpy(ascii, ASCII_EMPTY, sizeof(ASCII_EMPTY));
        } else {
            memcpy(ascii, ASCII_MAP[c], sizeof(ASCII_MAP[c]));
        }
        
        // Set the character in the array
        if (character != '#' && character != ' ')
            for (int j = 0; j < 8; j++)
                for (int k = 0; k < 5; k++)
                    if (ascii[j][k] == '#')
                        ascii[j][k] = character;

        Canvas_drawAscii(x + (i * 5), y, ascii);
    }
}