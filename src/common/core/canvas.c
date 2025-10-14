#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/util.h"

// Core Functions

#define _CANVAS_MUTEX 7

void Canvas_setChar(int x, int y, char c) {
    if (x < 0) return;
    if (y < 0) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    Canvas_setCursor(x, y);
    putchar(c);
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_setAnsiCurrent(const char* ansi) {
    if (ansi == 0) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("%s", ansi);
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_setAnsi(int x, int y, const char* ansi) {
    if (x < 0) return;
    if (y < 0) return;
    if (ansi == 0) return;

    Canvas_setCursor(x, y);
    Canvas_setAnsiCurrent(ansi);
}

// Utility Functions

void Canvas_resetFormat() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[0m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_setForeground(int rgb) {
    if (rgb < 0) return;

    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    char* ansi = malloc(22);
    snprintf(ansi, 22, "\033[38;2;%d;%d;%dm", r, g, b);
    Canvas_setAnsiCurrent(ansi);
    free(ansi);

    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_setBackground(int rgb) {
    if (rgb < 0) return;

    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    char* ansi = malloc(22);
    snprintf(ansi, 22, "\033[48;2;%d;%d;%dm", r, g, b);
    Canvas_setAnsiCurrent(ansi);
    free(ansi);

    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_setColor8(int color) {
    if (color < 30 || color > 107) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    char* ansi = malloc(9);
    snprintf(ansi, 9, "\033[%dm", color);
    Canvas_setAnsiCurrent(ansi);
    free(ansi);

    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_setForeground256(int color) {
    if (color < 0 || color > 255) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    char* ansi = malloc(14);
    snprintf(ansi, 14, "\033[38;5;%dm", color);
    Canvas_setAnsiCurrent(ansi);
    free(ansi);

    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_setBackground256(int color) {
    if (color < 0 || color > 255) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    char* ansi = malloc(14);
    snprintf(ansi, 14, "\033[48;5;%dm", color);
    Canvas_setAnsiCurrent(ansi);
    free(ansi);

    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableBold() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[1m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableBold() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[22m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableDim() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[2m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableDim() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[22m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableItalic() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[3m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableItalic() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[23m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableUnderline() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[4m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableUnderline() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[24m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableBlink() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[5m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableBlink() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[25m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableInvert() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[7m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableInvert() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[27m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableHidden() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[8m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableHidden() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[28m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_enableStrikethrough() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[9m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_disableStrikethrough() {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    printf("\033[29m");
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

// Utility Functions - Shapes

void Canvas_hLine(int x, int y, int width, char c) {
    if (width < 1) return;
    if (x < 0) return;
    if (y < 0) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    for (int i = 0; i < width; i++) {
        Canvas_setCursor(x + i, y);
        putchar(c);
    }

    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_vLine(int x, int y, int height, char c) {
    if (height < 1) return;
    if (x < 0) return;
    if (y < 0) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    for (int i = 0; i < height; i++) {
        Canvas_setCursor(x, y + i);
        putchar(c);
    }

    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
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
        }
        else {
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
    int xr2 = xradius * xradius;
    int yr2 = yradius * yradius;
    int twoxr2 = 2 * xr2;
    int twoyr2 = 2 * yr2;
    int p;
    int px = 0;
    int py = twoxr2 * y1;

    p = yr2 - (xr2 * yradius) + (0.25 * xr2);
    while (px < py) {
        Canvas_setChar(x + x1, y + y1, c);
        Canvas_setChar(x - x1, y + y1, c);
        Canvas_setChar(x + x1, y - y1, c);
        Canvas_setChar(x - x1, y - y1, c);

        x1++;
        px += twoyr2;

        if (p < 0) {
            p += yr2 + px;
        }
        else {
            y1--;
            py -= twoxr2;
            p += yr2 + px - py;
        }
    }

    p = yr2 * (x1 + 0.5) * (x1 + 0.5) + xr2 * (y1 - 1) * (y1 - 1) - xr2 * yr2;
    while (y1 >= 0) {
        Canvas_setChar(x + x1, y + y1, c);
        Canvas_setChar(x - x1, y + y1, c);
        Canvas_setChar(x + x1, y - y1, c);
        Canvas_setChar(x - x1, y - y1, c);

        y1--;
        py -= twoxr2;

        if (p > 0) {
            p += xr2 - py;
        }
        else {
            x1++;
            px += twoyr2;
            p += xr2 - py + px;
        }
    }
}

void Canvas_fillEllipse(int x, int y, int xradius, int yradius, char c) {
    for (int dx = -xradius; dx <= xradius; dx++) {
        for (int dy = -yradius; dy <= yradius; dy++) {
            double ellipseEquation = (dx * dx) / (double) (xradius * xradius) +
                                     (dy * dy) / (double) (yradius * yradius);

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
    int prevY = y;          // + (radius * sin(0));

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
    int* vx = malloc(sides * sizeof(int));
    int* vy = malloc(sides * sizeof(int));

    if (!vx || !vy) {
        free(
            vx
        ); // Free what was allocated in case one succeeded and one failed
        free(vy);
        return;
    }

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
        int* intersections = calloc(sides, sizeof(int));
        if (!intersections) {
            free(vx);
            free(vy);
            return; // Clean up and exit on allocation failure
        }
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
                for (int xFill = intersections[i];
                     xFill <= intersections[i + 1]; xFill++) {
                    Canvas_setChar(xFill, scanY, c);
                }
            }
        }

        free(intersections);
    }

    free(vx);
    free(vy);
}

void Canvas_quad(int x, int y, int x1, int y1, int x2, int y2, char c) {
    if (x < 0 || y < 0) return;

    double t;
    for (t = 0; t <= 1; t += 0.01) {
        double xt = (1 - t) * (1 - t) * x + 2 * (1 - t) * t * x1 + t * t * x2;
        double yt = (1 - t) * (1 - t) * y + 2 * (1 - t) * t * y1 + t * t * y2;
        Canvas_setChar((int) xt, (int) yt, c);
    }
}

void Canvas_cubic(
    int x, int y, int x1, int y1, int x2, int y2, int x3, int y3, char c
) {
    if (x < 0 || y < 0) return;

    double t;
    for (t = 0; t <= 1; t += 0.01) {
        double xt = (1 - t) * (1 - t) * (1 - t) * x +
                    3 * (1 - t) * (1 - t) * t * x1 + 3 * (1 - t) * t * t * x2 +
                    t * t * t * x3;
        double yt = (1 - t) * (1 - t) * (1 - t) * y +
                    3 * (1 - t) * (1 - t) * t * y1 + 3 * (1 - t) * t * t * y2 +
                    t * t * t * y3;
        Canvas_setChar((int) xt, (int) yt, c);
    }
}

void Canvas_arc(
    int x1, int y1, int rx, int ry, double xrot, int arcflag, int sweepflag,
    int x2, int y2, char c
) {
    if (rx == 0) return;
    if (ry == 0) return;
    if (x1 < 0) return;
    if (y1 < 0) return;
    if (x2 < 0) return;
    if (y2 < 0) return;
    if (arcflag < 0) return;
    if (arcflag > 1) return;
    if (sweepflag < 0) return;
    if (sweepflag > 1) return;

    double phi = xrot * M_PI / 180.0;
    double cosp = cos(phi);
    double sinp = sin(phi);

    // Step 1: Compute (x1', y1')
    double dx = (x1 - x2) / 2.0;
    double dy = (y1 - y2) / 2.0;
    double x1p = cosp * dx + sinp * dy;
    double y1p = -sinp * dx + cosp * dy;

    // Ensure radii are large enough
    double rx2 = rx * rx;
    double ry2 = ry * ry;
    double x1p2 = x1p * x1p;
    double y1p2 = y1p * y1p;
    double lambda = (x1p2 / rx2) + (y1p2 / ry2);
    if (lambda > 1) {
        double scale = sqrt(lambda);
        rx *= scale;
        ry *= scale;
        rx2 = rx * rx;
        ry2 = ry * ry;
    }

    // Step 2: Compute (cx', cy')
    double sign = (arcflag == sweepflag) ? -1 : 1;
    double num = rx2 * ry2 - rx2 * y1p2 - ry2 * x1p2;
    double denom = rx2 * y1p2 + ry2 * x1p2;
    double factor = sign * sqrt(fmax(0, num / denom));
    double cxp = factor * (rx * y1p) / ry;
    double cyp = factor * (-ry * x1p) / rx;

    // Step 3: Compute (cx, cy)
    double cx = cosp * cxp - sinp * cyp + (x1 + x2) / 2.0;
    double cy = sinp * cxp + cosp * cyp + (y1 + y2) / 2.0;

    // Step 4: Compute start and sweep angles
    double theta1 = atan2((y1p - cyp) / ry, (x1p - cxp) / rx);
    double theta2 = atan2((-y1p - cyp) / ry, (-x1p - cxp) / rx) - theta1;
    if (sweepflag && theta2 < 0)
        theta2 += 2 * M_PI;
    else if (!sweepflag && theta2 > 0)
        theta2 -= 2 * M_PI;

    // Step 5: Draw arc as line segments
    int prevX = x1;
    int prevY = y1;

    for (int i = 1; i <= 100; i++) {
        double t = (double) i / 100;
        double angle = theta1 + t * theta2;
        double xt = rx * cos(angle);
        double yt = ry * sin(angle);

        // Rotate and translate back
        double x = cosp * xt - sinp * yt + cx;
        double y = sinp * xt + cosp * yt + cy;

        Canvas_line(prevX, prevY, (int) (x + 0.5), (int) (y + 0.5), c);
        prevX = (int) (x + 0.5);
        prevY = (int) (y + 0.5);
    }
}

// Utility Functions - Text

void Canvas_drawText(int x, int y, char* text) {
    if (x < 0) return;
    if (y < 0) return;
    if (text == 0) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    Canvas_setCursor(x, y);
    printf("%s", text);
    fflush(stdout);

    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Canvas_drawAscii(int x, int y, char ascii[8][5]) {
    if (x < 0) return;
    if (y < 0) return;
    if (ascii == 0) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            Canvas_setCursor(x + j, y + i);
            putchar(ascii[i][j]);
        }
    }

    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

#pragma region ASCII Art

char ASCII_EMPTY[8][5] = {{' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
                          {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
                          {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
                          {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_UPPER_A[8][5] = {
    {' ', ' ', '#', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_A[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_B[8][5] = {
    {' ', '#', '#', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_B[8][5] = {
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_C[8][5] = {
    {' ', ' ', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_C[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_D[8][5] = {
    {' ', '#', '#', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_D[8][5] = {
    {' ', ' ', ' ', '#', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '}, {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_E[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_E[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_F[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_F[8][5] = {
    {' ', ' ', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_G[8][5] = {
    {' ', ' ', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_G[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', '#', ' ', ' '}
};

char ASCII_UPPER_H[8][5] = {
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_H[8][5] = {
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_I[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_I[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_J[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_J[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '}
};

char ASCII_UPPER_K[8][5] = {
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', ' ', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_K[8][5] = {
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', '#', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_L[8][5] = {
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_L[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_M[8][5] = {
    {'#', ' ', ' ', ' ', '#'}, {'#', '#', ' ', '#', '#'},
    {'#', ' ', '#', ' ', '#'}, {'#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#'}, {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_M[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {'#', '#', '#', '#', '#'}, {'#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#'}, {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_N[8][5] = {
    {' ', '#', ' ', ' ', '#'}, {' ', '#', ' ', ' ', '#'},
    {' ', '#', '#', ' ', '#'}, {' ', '#', ' ', '#', '#'},
    {' ', '#', ' ', ' ', '#'}, {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_N[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_O[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_O[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'}, {' ', '#', ' ', ' ', '#'},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_P[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_P[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '}
};

char ASCII_UPPER_Q[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', ' ', '#'}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_Q[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '}, {' ', ' ', ' ', '#', ' '}
};

char ASCII_UPPER_R[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', '#', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', ' ', '#'},
    {' ', '#', ' ', ' ', '#'}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_R[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_S[8][5] = {
    {' ', '#', '#', '#', '#'}, {'#', ' ', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', ' '}, {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', ' ', '#'}, {'#', ' ', ' ', ' ', '#'},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_S[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', '#', '#', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', ' ', '#', '#', ' '},
    {' ', ' ', ' ', ' ', '#'}, {' ', '#', ' ', ' ', '#'},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_T[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_T[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_U[8][5] = {
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_U[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_V[8][5] = {
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_V[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_W[8][5] = {
    {'#', ' ', ' ', ' ', '#'}, {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#'}, {'#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', '#'}, {'#', '#', ' ', '#', '#'},
    {'#', ' ', ' ', ' ', '#'}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_W[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {'#', ' ', ' ', ' ', '#'}, {'#', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', '#'}, {'#', '#', ' ', '#', '#'},
    {'#', ' ', ' ', ' ', '#'}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_X[8][5] = {
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_X[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_UPPER_Y[8][5] = {
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_Y[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', ' ', '#', ' ', ' '}, {' ', '#', ' ', ' ', ' '}
};

char ASCII_UPPER_Z[8][5] = {
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', '#', ' '},
    {' ', ' ', ' ', '#', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_LOWER_Z[8][5] = {
    {' ', ' ', ' ', ' ', ' '}, {' ', '#', '#', '#', ' '},
    {' ', ' ', ' ', '#', ' '}, {' ', ' ', '#', ' ', ' '},
    {' ', '#', ' ', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
    {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}
};

char ASCII_ZERO[8][5] = {{' ', '#', '#', '#', ' '}, {' ', '#', '#', '#', ' '},
                         {' ', '#', ' ', '#', ' '}, {' ', '#', ' ', '#', ' '},
                         {' ', '#', ' ', '#', ' '}, {' ', '#', '#', '#', ' '},
                         {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_ONE[8][5] = {{' ', ' ', '#', ' ', ' '}, {' ', '#', '#', ' ', ' '},
                        {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
                        {' ', ' ', '#', ' ', ' '}, {' ', ' ', '#', ' ', ' '},
                        {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_TWO[8][5] = {{' ', '#', '#', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                        {' ', ' ', ' ', '#', ' '}, {' ', ' ', '#', ' ', ' '},
                        {' ', '#', ' ', ' ', ' '}, {'#', ' ', ' ', ' ', ' '},
                        {'#', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_THREE[8][5] = {{' ', '#', '#', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                          {' ', ' ', ' ', '#', ' '}, {' ', ' ', '#', ' ', ' '},
                          {' ', ' ', ' ', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                          {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_FOUR[8][5] = {{' ', ' ', '#', ' ', ' '}, {' ', '#', ' ', '#', ' '},
                         {'#', ' ', ' ', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                         {'#', '#', '#', '#', '#'}, {' ', ' ', ' ', '#', ' '},
                         {' ', ' ', ' ', '#', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_FIVE[8][5] = {{'#', '#', '#', '#', ' '}, {'#', ' ', ' ', ' ', ' '},
                         {'#', '#', '#', '#', ' '}, {' ', ' ', ' ', '#', ' '},
                         {' ', ' ', ' ', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                         {' ', '#', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_SIX[8][5] = {{' ', '#', '#', '#', ' '}, {'#', ' ', ' ', ' ', ' '},
                        {'#', ' ', ' ', ' ', ' '}, {'#', '#', '#', '#', ' '},
                        {'#', ' ', ' ', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                        {' ', '#', '#', '#', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_SEVEN[8][5] = {{'#', '#', '#', '#', ' '}, {' ', ' ', ' ', '#', ' '},
                          {' ', ' ', '#', ' ', ' '}, {' ', '#', ' ', ' ', ' '},
                          {'#', ' ', ' ', ' ', ' '}, {'#', ' ', ' ', ' ', ' '},
                          {'#', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_EIGHT[8][5] = {{' ', '#', '#', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                          {'#', ' ', ' ', '#', ' '}, {' ', '#', '#', ' ', ' '},
                          {'#', ' ', ' ', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                          {' ', '#', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}};

char ASCII_NINE[8][5] = {{' ', '#', '#', '#', ' '}, {'#', ' ', ' ', '#', ' '},
                         {'#', ' ', ' ', '#', ' '}, {' ', '#', '#', '#', ' '},
                         {' ', ' ', ' ', '#', ' '}, {' ', ' ', ' ', '#', ' '},
                         {'#', '#', '#', ' ', ' '}, {' ', ' ', ' ', ' ', ' '}};

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
    if (x < 0 || y < 0) return;
    if (!text) return;

    int textLen = strlen(text); // Calculate once, store in variable
    if (textLen == 0) return;

    _initAsciiText();

    for (int i = 0; i < textLen; i++) {
        char c = text[i];
        char ascii[8][5];

        if (c > 127) {
            memcpy(ascii, ASCII_EMPTY, sizeof(ASCII_EMPTY));
        }
        else {
            memcpy(ascii, ASCII_MAP[c], sizeof(ASCII_MAP[c]));
        }

        // Set the character in the array
        if (character != '#' && character != ' ')
            for (int j = 0; j < 8; j++)
                for (int k = 0; k < 5; k++)
                    if (ascii[j][k] == '#') ascii[j][k] = character;

        Canvas_drawAscii(x + (i * 5), y, ascii);
    }
}
