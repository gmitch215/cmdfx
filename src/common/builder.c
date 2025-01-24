#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/builder.h"

#pragma region Character Builder

int getArrayWidth(char** array) {
    if (array == 0) return 0;
    return strlen(array[0]);
}

int getArrayHeight(char** array) {
    if (array == 0) return 0;
    
    int height;
    while (array[height] != 0) height++;

    return height;
}

// Core Functions (Character)

char** CharBuilder_create(int width, int height) {
    if (width <= 0 || height <= 0) return 0;

    char** array = (char**) malloc(sizeof(char*) * (height + 1));
    if (array == 0) return 0;

    for (int i = 0; i < height; i++) {
        array[i] = (char*) malloc(sizeof(char) * (width + 1));
        if (array[i] == 0) {
            for (int j = 0; j < i; j++) free(array[j]);
            free(array);
            return 0;
        }

        for (int j = 0; j < width; j++) array[i][j] = ' ';
    }

    array[height] = 0;
    array[height][width] = 0;

    return array;
}

int CharBuilder_setChar(char** array, int x, int y, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (x >= width || y >= height) return -1;

    array[y][x] = c;
    return 0;
}

int CharBuilder_hLine(char** array, int x, int y, int width, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y][x + width - 1] == 0) return -1;

    for (int i = 0; i < width; i++) {
        if (array[y][x + i] == 0) break;
        array[y][x + i] = c;
    }

    return 0;
}

int CharBuilder_vLine(char** array, int x, int y, int height, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y + height - 1] == 0) return -1;

    for (int i = 0; i < height; i++) {
        if (array[y + i] == 0) break;
        array[y + i][x] = c;
    }

    return 0;
}

int CharBuilder_rect(char** array, int x, int y, int width, int height, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y][x + width - 1] == 0) return -1;
    if (array[y + height - 1] == 0) return -1;

    for (int i = 0; i < width; i++) {
        if (array[y][x + i] == 0) break;
        array[y][x + i] = c;
        array[y + height - 1][x + i] = c;
    }

    for (int i = 0; i < height; i++) {
        if (array[y + i] == 0) break;
        array[y + i][x] = c;
        array[y + i][x + width - 1] = c;
    }

    return 0;
}

int CharBuilder_fillRect(char** array, int x, int y, int width, int height, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y][x + width - 1] == 0) return -1;
    if (array[y + height - 1] == 0) return -1;

    for (int i = 0; i < width; i++) {
        if (array[y][x + i] == 0) break;
        for (int j = 0; j < height; j++) {
            if (array[y + j] == 0) break;
            array[y + j][x + i] = c;
        }
    }

    return 0;
}

int CharBuilder_circle(char** array, int x, int y, int radius, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    
    if (array[y + radius] == 0) return -1;
    if (array[y - radius] == 0) return -1;
    if (array[y][x + radius] == 0) return -1;
    if (array[y][x - radius] == 0) return -1;

    int x1 = 0;
    int y1 = radius;
    int d = 1 - radius;

    while (x1 <= y1) {
        CharBuilder_setChar(array, x + x1, y + y1, c);
        CharBuilder_setChar(array, x - x1, y + y1, c);
        CharBuilder_setChar(array, x + x1, y - y1, c);
        CharBuilder_setChar(array, x - x1, y - y1, c);
        CharBuilder_setChar(array, x + y1, y + x1, c);
        CharBuilder_setChar(array, x - y1, y + x1, c);
        CharBuilder_setChar(array, x + y1, y - x1, c);
        CharBuilder_setChar(array, x - y1, y - x1, c);

        if (d < 0) {
            d += 2 * x1 + 3;
        } else {
            d += 2 * (x1 - y1) + 5;
            y1--;
        }
        x1++;
    }

    return 0;
}

int CharBuilder_fillCircle(char** array, int x, int y, int radius, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;

    if (array[y + radius] == 0) return -1;
    if (array[y - radius] == 0) return -1;
    if (array[y][x + radius] == 0) return -1;
    if (array[y][x - radius] == 0) return -1;

    int radiusSq = radius * radius;

    for (int y1 = -radius; y1 <= radius; y1++) {
        int y1Sq = y1 * y1;
        int x1 = 0;
        while (x1 * x1 + y1Sq <= radiusSq) {
            CharBuilder_setChar(array, x + x1, y + y1, c);
            CharBuilder_setChar(array, x - x1, y + y1, c);
            CharBuilder_setChar(array, x + x1, y - y1, c);
            CharBuilder_setChar(array, x - x1, y - y1, c);
            x1++;
        }
    }

    return 0;
}

int CharBuilder_ellipse(char** array, int x, int y, int xradius, int yradius, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;

    if (array[y + yradius] == 0) return -1;
    if (array[y - yradius] == 0) return -1;
    if (array[y][x + xradius] == 0) return -1;
    if (array[y][x - xradius] == 0) return -1;

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
        CharBuilder_setChar(array, x + x1, y + y1, c);
        CharBuilder_setChar(array, x - x1, y + y1, c);
        CharBuilder_setChar(array, x + x1, y - y1, c);
        CharBuilder_setChar(array, x - x1, y - y1, c);

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
        CharBuilder_setChar(array, x + x1, y + y1, c);
        CharBuilder_setChar(array, x - x1, y + y1, c);
        CharBuilder_setChar(array, x + x1, y - y1, c);
        CharBuilder_setChar(array, x - x1, y - y1, c);

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

int CharBuilder_fillEllipse(char** array, int x, int y, int xradius, int yradius, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (x + xradius >= width || y + yradius >= height) return -1;
    if (x - xradius < 0 || y - yradius < 0) return -1;

    int xradiusSq = xradius * xradius;
    int yradiusSq = yradius * yradius;

    for (int dy = -yradius; dy <= yradius; dy++) {
        for (int dx = -xradius; dx <= xradius; dx++) {
            double ellipseEquation = 
                (dx * dx) / (double)(xradiusSq) + 
                (dy * dy) / (double)(yradiusSq);

            if (ellipseEquation <= 1.0) {
                int px = x + dx;
                int py = y + dy;

                CharBuilder_setChar(array, px, py, c);
            }
        }
    }
}

int CharBuilder_line(char** array, int x1, int y1, int x2, int y2, char c) {
    if (array == 0) return -1;
    if (x1 < 0 || y1 < 0) return -1;

    if (array[y1] == 0) return -1;
    if (array[y1][x1] == 0) return -1;

    if (array[y2] == 0) return -1;
    if (array[y2][x2] == 0) return -1;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        CharBuilder_setChar(array, x1, y1, c);
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

    return 0;
}

int CharBuilder_text(char** array, int x, int y, char* text) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;

    int i = 0;
    while (text[i] != 0) {
        if (array[y][x] == 0) {
            x = 0;
            y++;
        }

        if (array[y] == 0) return -1;

        array[y][x] = text[i];
        x++;
        i++;
    }

    return 0;
}

// Utility Functions - Sizing

int CharBuilder_resize0(char** array, int width, int height, char padding) {
    int oldWidth = getArrayWidth(array);
    int oldHeight = getArrayHeight(array);

    char** newArray = CharBuilder_create(width, height);
    if (newArray == 0) return -1;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < oldWidth && y < oldHeight)
                newArray[y][x] = array[y][x];
            else
                newArray[y][x] = padding;
        }
    }

    *array = *newArray;

    return 0;
}

int CharBuilder_resize(char** array, int width, int height) {
    if (array == 0) return -1;
    if (width <= 0 || height <= 0) return -1;

    CharBuilder_resize0(array, width, height, ' ');
    return 0;
}

int CharBuilder_resizeWithPadding(char** array, int width, int height, char padding) {
    if (array == 0) return -1;
    if (width <= 0 || height <= 0) return -1;

    CharBuilder_resize0(array, width, height, padding);
    return 0;
}

int CharBuilder_center0(char** array) {
    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    int x = 0;
    int y = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (array[i][j] < ' ') {
                x = j;
                y = i;
                break;
            }
        }
    }

    int xoffset = (width - x) / 2;
    int yoffset = (height - y) / 2;

    char** newArray = CharBuilder_create(width, height);
    if (newArray == 0) return -1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i - yoffset >= 0 && i - yoffset < height && j - xoffset >= 0 && j - xoffset < width)
                newArray[i][j] = array[i - yoffset][j - xoffset];
            else
                newArray[i][j] = ' ';
        }
    }

    *array = *newArray;

    return 0;
}

int CharBuilder_center(char** array) {
    if (array == 0) return -1;

    CharBuilder_center0(array);
    return 0;
}

int CharBuilder_resizeAndCenter(char** array, int width, int height) {
    if (array == 0) return -1;
    if (width <= 0 || height <= 0) return -1;

    CharBuilder_resize0(array, width, height, ' ');
    CharBuilder_center0(array);
    return 0;
}

#pragma endregion

#pragma region ANSI Builder

int getAnsiArrayWidth(char*** array) {
    if (array == 0) return 0;

    int width;
    while (array[0][width] != 0) width++;

    return width;
}

int getAnsiArrayHeight(char*** array) {
    if (array == 0) return 0;

    int height;
    while (array[height] != 0) height++;

    return height;
}

// Core Functions (ANSI)

char*** AnsiBuilder_create(int width, int height) {
    if (width <= 0 || height <= 0) return 0;

    char*** array = (char***) malloc(sizeof(char**) * (height + 1));
    if (array == 0) return 0;

    for (int i = 0; i < height; i++) {
        array[i] = (char**) malloc(sizeof(char*) * (width + 1));
        if (array[i] == 0) {
            for (int j = 0; j < i; j++) free(array[j]);
            free(array);
            return 0;
        }

        for (int j = 0; j < width; j++) array[i][j] = 0;
    }

    array[height] = 0;
    array[height][width] = 0;

    return array;
}

int AnsiBuilder_setAnsi(char*** array, int x, int y, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    if (x >= width || y >= height) return -1;

    array[y][x] = c;
    return 0;
}

int AnsiBuilder_hLine(char*** array, int x, int y, int width, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y][x + width - 1] == 0) return -1;

    for (int i = 0; i < width; i++) {
        if (array[y][x + i] == 0) break;
        array[y][x + i] = c;
    }

    return 0;
}

int AnsiBuilder_vLine(char*** array, int x, int y, int height, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y + height - 1] == 0) return -1;

    for (int i = 0; i < height; i++) {
        if (array[y + i] == 0) break;
        array[y + i][x] = c;
    }

    return 0;
}

int AnsiBuilder_rect(char*** array, int x, int y, int width, int height, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y][x + width - 1] == 0) return -1;
    if (array[y + height - 1] == 0) return -1;

    for (int i = 0; i < width; i++) {
        if (array[y][x + i] == 0) break;
        array[y][x + i] = c;
        array[y + height - 1][x + i] = c;
    }

    for (int i = 0; i < height; i++) {
        if (array[y + i] == 0) break;
        array[y + i][x] = c;
        array[y + i][x + width - 1] = c;
    }

    return 0;
}

int AnsiBuilder_fillRect(char*** array, int x, int y, int width, int height, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    if (array[y][x + width - 1] == 0) return -1;
    if (array[y + height - 1] == 0) return -1;

    for (int i = 0; i < width; i++) {
        if (array[y][x + i] == 0) break;
        for (int j = 0; j < height; j++) {
            if (array[y + j] == 0) break;
            array[y + j][x + i] = c;
        }
    }

    return 0;
}

int AnsiBuilder_circle(char*** array, int x, int y, int radius, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;
    
    if (array[y + radius] == 0) return -1;
    if (array[y - radius] == 0) return -1;
    if (array[y][x + radius] == 0) return -1;
    if (array[y][x - radius] == 0) return -1;

    int x1 = 0;
    int y1 = radius;
    int d = 1 - radius;

    while (x1 <= y1) {
        AnsiBuilder_setAnsi(array, x + x1, y + y1, c);
        AnsiBuilder_setAnsi(array, x - x1, y + y1, c);
        AnsiBuilder_setAnsi(array, x + x1, y - y1, c);
        AnsiBuilder_setAnsi(array, x - x1, y - y1, c);
        AnsiBuilder_setAnsi(array, x + y1, y + x1, c);
        AnsiBuilder_setAnsi(array, x - y1, y + x1, c);
        AnsiBuilder_setAnsi(array, x + y1, y - x1, c);
        AnsiBuilder_setAnsi(array, x - y1, y - x1, c);

        if (d < 0) {
            d += 2 * x1 + 3;
        } else {
            d += 2 * (x1 - y1) + 5;
            y1--;
        }
        x1++;
    }

    return 0;
}

int AnsiBuilder_fillCircle(char*** array, int x, int y, int radius, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;

    if (array[y + radius] == 0) return -1;
    if (array[y - radius] == 0) return -1;
    if (array[y][x + radius] == 0) return -1;
    if (array[y][x - radius] == 0) return -1;

    int radiusSq = radius * radius;

    for (int y1 = -radius; y1 <= radius; y1++) {
        int y1Sq = y1 * y1;
        int x1 = 0;
        while (x1 * x1 + y1Sq <= radiusSq) {
            AnsiBuilder_setAnsi(array, x + x1, y + y1, c);
            AnsiBuilder_setAnsi(array, x - x1, y + y1, c);
            AnsiBuilder_setAnsi(array, x + x1, y - y1, c);
            AnsiBuilder_setAnsi(array, x - x1, y - y1, c);
            x1++;
        }
    }

    return 0;
}

int AnsiBuilder_ellipse(char*** array, int x, int y, int xradius, int yradius, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (array[y] == 0) return -1;
    if (array[y][x] == 0) return -1;

    if (array[y + yradius] == 0) return -1;
    if (array[y - yradius] == 0) return -1;
    if (array[y][x + xradius] == 0) return -1;
    if (array[y][x - xradius] == 0) return -1;

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
        AnsiBuilder_setAnsi(array, x + x1, y + y1, c);
        AnsiBuilder_setAnsi(array, x - x1, y + y1, c);
        AnsiBuilder_setAnsi(array, x + x1, y - y1, c);
        AnsiBuilder_setAnsi(array, x - x1, y - y1, c);

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
        AnsiBuilder_setAnsi(array, x + x1, y + y1, c);
        AnsiBuilder_setAnsi(array, x - x1, y + y1, c);
        AnsiBuilder_setAnsi(array, x + x1, y - y1, c);
        AnsiBuilder_setAnsi(array, x - x1, y - y1, c);

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

int AnsiBuilder_fillEllipse(char*** array, int x, int y, int xradius, int yradius, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    if (x + xradius >= width || y + yradius >= height) return -1;
    if (x - xradius < 0 || y - yradius < 0) return -1;

    int xradiusSq = xradius * xradius;
    int yradiusSq = yradius * yradius;

    for (int dy = -yradius; dy <= yradius; dy++) {
        for (int dx = -xradius; dx <= xradius; dx++) {
            double ellipseEquation = 
                (dx * dx) / (double)(xradiusSq) + 
                (dy * dy) / (double)(yradiusSq);

            if (ellipseEquation <= 1.0) {
                int px = x + dx;
                int py = y + dy;

                AnsiBuilder_setAnsi(array, px, py, c);
            }
        }
    }
}

int AnsiBuilder_line(char*** array, int x1, int y1, int x2, int y2, char* c) {
    if (array == 0) return -1;
    if (x1 < 0 || y1 < 0) return -1;

    if (array[y1] == 0) return -1;
    if (array[y1][x1] == 0) return -1;

    if (array[y2] == 0) return -1;
    if (array[y2][x2] == 0) return -1;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        AnsiBuilder_setAnsi(array, x1, y1, c);
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

    return 0;
}

#pragma endregion