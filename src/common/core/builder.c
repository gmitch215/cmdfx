#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "cmdfx/core/builder.h"

#pragma region Character Builder

int getArrayWidth(char** array) {
    if (array == 0) return 0;
    return strlen(array[0]);
}

int getArrayHeight(char** array) {
    if (array == 0) return 0;
    
    int height = 0;
    while (array[height] != 0) height++;

    return height;
}

// Core Functions (Character)

char** CharBuilder_create(int width, int height) {
    if (width <= 0 || height <= 0) return 0;

    char** array = (char**) calloc(height + 1, sizeof(char*));
    if (array == 0) return 0;

    for (int i = 0; i < height; i++) {
        array[i] = (char*) calloc(width + 1, sizeof(char));
        if (array[i] == 0) {
            for (int j = 0; j < i; j++) free(array[j]);
            free(array);
            return 0;
        }

        for (int j = 0; j < width; j++) array[i][j] = ' ';
    }

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

int CharBuilder_fill(char** array, char c) {
    if (array == 0) return -1;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    return CharBuilder_fillRect(array, 0, 0, width, height, c);
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

    return 0;
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

    return 0;
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

int CharBuilder_polygon(char** array, int x, int y, int sides, int radius, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;
    if (radius < 1) return -1;
    if (sides < 3) return -1;

    double angleStep = 2 * M_PI / sides;
    
    int prevX = x + radius; // * cos(0);
    int prevY = y; // + (radius * sin(0));

    for (int i = 1; i <= sides; i++) {
        int nextX = x + radius * cos(i * angleStep);
        int nextY = y + radius * sin(i * angleStep);

        CharBuilder_line(array, prevX, prevY, nextX, nextY, c);

        prevX = nextX;
        prevY = nextY;
    }

    return 0;
}

int CharBuilder_fillPolygon(char** array, int x, int y, int sides, int radius, char c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;
    if (radius < 1) return -1;
    if (sides < 3) return -1;

    double angleStep = 2 * M_PI / sides;

    int* vx = malloc(sides * sizeof(int));
    int* vy = malloc(sides * sizeof(int));

    if (!vx || !vy) return -1;

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
                    CharBuilder_setChar(array, xFill, scanY, c);
                }
            }
        }

        free(intersections);
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

// Utility Functions - Transformation

int CharBuilder_rotate(char** array, double radians) {
    if (array == 0) return -1;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (width <= 0 || height <= 0) return -1;

    int cx = width / 2;
    int cy = height / 2;

    char** tempArray = (char**) malloc(height * sizeof(char*));
    if (tempArray == 0) return -1;

    for (int i = 0; i < height; i++) {
        tempArray[i] = (char*) malloc(width * sizeof(char));
        if (tempArray[i] == 0) {
            for (int j = 0; j < i; j++) free(tempArray[j]);
            free(tempArray);
            return -1;
        }
    }

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            int newX = (int) (cos(radians) * (x - cx) - sin(radians) * (y - cy) + cx);
            int newY = (int) (sin(radians) * (x - cx) + cos(radians) * (y - cy) + cy);

            if (newX >= 0 && newX < width && newY >= 0 && newY < height)
                tempArray[newY][newX] = array[y][x];
            else
                tempArray[newY][newX] = ' '; // Fill empty spaces outside bounds
        }

    // Copy rotated values
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            array[y][x] = tempArray[y][x];
        }

    // Free the temporary array
    for (int i = 0; i < height; i++) {
        free(tempArray[i]);
    }
    free(tempArray);

    return 0;
}

double CharBuilder_getRotationAngle(char** array) {
    if (array == 0) return 0.0;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (width <= 0 || height <= 0) return 0.0;
    
    double cx = width / 2.0;
    double cy = height / 2.0;
    
    double sumX = 0, sumY = 0;
    int whitespaceCount = 0;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (array[y][x] == ' ') {
                sumX += x;
                sumY += y;
                whitespaceCount++;
            }
        }
    }

    if (whitespaceCount == 0) return 0.0;

    // Compute centroid
    double avgX = sumX / whitespaceCount;
    double avgY = sumY / whitespaceCount;
    
    // Compute moment of inertia
    double sumXY = 0, sumXX = 0, sumYY = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (array[y][x] == ' ') {
                double dx = x - avgX;
                double dy = y - avgY;
                sumXX += dx * dx;
                sumYY += dy * dy;
                sumXY += dx * dy;
            }
        }
    }

    double theta = 0.5 * atan2(2 * sumXY, sumXX - sumYY);

    return theta;
}

int CharBuilder_hFlip(char** array) {
    if (array == 0) return -1;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (width <= 0 || height <= 0) return -1;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            // Swap columns
            char temp = array[y][x];
            array[y][x] = array[y][width - 1 - x];
            array[y][width - 1 - x] = temp;
        }
    }

    return 0;
}

int CharBuilder_vFlip(char** array) {
    if (array == 0) return -1;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (width <= 0 || height <= 0) return -1;

    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            // Swap rows
            char temp = array[y][x];
            array[y][x] = array[height - 1 - y][x];
            array[height - 1 - y][x] = temp;
        }
    }

    return 0;
}

char** CharBuilder_transpose(char** array) {
    if (array == 0) return 0;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (width <= 0 || height <= 0) return 0;

    char** transposed = (char**) malloc(width * sizeof(char*));
    if (transposed == 0) return 0;
    
    for (int i = 0; i < width; i++) {
        transposed[i] = (char*) malloc(height * sizeof(char));
        if (transposed[i] == 0) {
            for (int j = 0; j < i; j++) free(transposed[j]);
            free(transposed);
            return 0;
        }
    }

    // Transpose the array
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            transposed[x][y] = array[y][x];
        }
    }

    // Free the original array
    for (int i = 0; i < height; i++) {
        free(array[i]);
    }
    free(array);

    return transposed;
}

int CharBuilder_replaceAll(char** array, char find, char replace) {
    if (array == 0) return -1;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    int count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (array[y][x] == find) {
                array[y][x] = replace;
                count++;
            }
        }
    }

    return count;
}

char** CharBuilder_scale(char** array, double scale) {
    if (array == 0) return 0;
    if (scale <= 0) return 0;

    int width = getArrayWidth(array);
    int height = getArrayHeight(array);

    if (width <= 0 || height <= 0) return 0;

    char** scaled = (char**) malloc(height * scale * sizeof(char*));
    if (scaled == 0) return 0;

    for (int i = 0; i < height * scale; i++) {
        scaled[i] = (char*) malloc(width * scale * sizeof(char));

        if (scaled[i] == 0) {
            for (int j = 0; j < i; j++) free(scaled[j]);
            free(scaled);
            return 0;
        }
    }

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            for (int sy = 0; sy < scale; sy++)
                for (int sx = 0; sx < scale; sx++)
                    scaled[(int) (y * scale + sy)][(int) (x * scale + sx)] = array[y][x];

    // Free the original array
    for (int i = 0; i < height; i++) {
        free(array[i]);
    }
    free(array);

    return scaled;
}

#pragma endregion

#pragma region ANSI Builder

int getAnsiArrayWidth(char*** array) {
    if (array == 0) return 0;

    int width = 0;
    while (array[0][width] != 0) width++;

    return width;
}

int getAnsiArrayHeight(char*** array) {
    if (array == 0) return 0;

    int height = 0;
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

int AnsiBuilder_fill(char*** array, char* c) {
    if (array == 0) return -1;
    if (c == 0) return -1;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    return AnsiBuilder_fillRect(array, 0, 0, width, height, c);
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

    return 0;
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

    return 0;
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

int AnsiBuilder_polygon(char*** array, int x, int y, int sides, int radius, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;
    if (radius < 1) return -1;
    if (sides < 3) return -1;

    double angleStep = 2 * M_PI / sides;
    
    int prevX = x + radius; // * cos(0);
    int prevY = y; // + (radius * sin(0));

    for (int i = 1; i <= sides; i++) {
        int nextX = x + radius * cos(i * angleStep);
        int nextY = y + radius * sin(i * angleStep);

        AnsiBuilder_line(array, prevX, prevY, nextX, nextY, c);

        prevX = nextX;
        prevY = nextY;
    }

    return 0;
}

int AnsiBuilder_fillPolygon(char*** array, int x, int y, int sides, int radius, char* c) {
    if (array == 0) return -1;
    if (x < 0 || y < 0) return -1;
    if (radius < 1) return -1;
    if (sides < 3) return -1;

    double angleStep = 2 * M_PI / sides;

    int* vx = malloc(sides * sizeof(int));
    int* vy = malloc(sides * sizeof(int));

    if (!vx || !vy) return -1;

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
                    AnsiBuilder_setAnsi(array, xFill, scanY, c);
                }
            }
        }

        free(intersections);
    }

    return 0;
}

// Utility Functions - Transformation (ANSI)

int AnsiBuilder_rotate(char*** array, double radians) {
    if (array == 0) return -1;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    if (width <= 0 || height <= 0) return -1;

    int cx = width / 2;
    int cy = height / 2;

    char*** tempArray = (char***) malloc(height * sizeof(char**));
    if (tempArray == 0) return -1;

    for (int i = 0; i < height; i++) {
        tempArray[i] = (char**) calloc(width, sizeof(char*));
        if (tempArray[i] == 0) {
            for (int j = 0; j < i; j++) free(tempArray[j]);
            free(tempArray);
            return -1;
        }
    }

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            int newX = (int) (cos(radians) * (x - cx) - sin(radians) * (y - cy) + cx);
            int newY = (int) (sin(radians) * (x - cx) + cos(radians) * (y - cy) + cy);

            if (newX >= 0 && newX < width && newY >= 0 && newY < height)
                strcpy(tempArray[newY][newX], array[y][x]);
        }

    // Copy rotated values
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            strcpy(array[y][x], tempArray[y][x]);
        }

    // Free the temporary array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            free(tempArray[i][j]);
        }
        free(tempArray[i]);
    }
    free(tempArray);

    return 0;
}

int AnsiBuilder_hFlip(char*** array) {
    if (array == 0) return -1;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    if (width <= 0 || height <= 0) return -1;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            // Swap columns
            char* temp = array[y][x];
            array[y][x] = array[y][width - 1 - x];
            array[y][width - 1 - x] = temp;
        }
    }

    return 0;
}

int AnsiBuilder_vFlip(char*** array) {
    if (array == 0) return -1;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    if (width <= 0 || height <= 0) return -1;

    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            // Swap rows
            char* temp = array[y][x];
            array[y][x] = array[height - 1 - y][x];
            array[height - 1 - y][x] = temp;
        }
    }

    return 0;
}

char*** AnsiBuilder_transpose(char*** array) {
    if (array == 0) return 0;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    if (width <= 0 || height <= 0) return 0;

    char*** transposed = (char***) malloc(width * sizeof(char**));
    if (transposed == 0) return 0;
    
    for (int i = 0; i < width; i++) {
        transposed[i] = (char**) malloc(height * sizeof(char*));
        if (transposed[i] == 0) {
            for (int j = 0; j < i; j++) free(transposed[j]);
            free(transposed);
            return 0;
        }
    }

    // Transpose the array
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            strcpy(transposed[x][y], array[y][x]);
        }
    }

    // Free the original array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            free(array[i][j]);
        }
        free(array[i]);
    }

    return transposed;
}

int AnsiBuilder_replaceAll(char*** array, char* find, char* replace) {
    if (array == 0) return -1;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    int count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (strcmp(array[y][x], find) == 0) {
                strcpy(array[y][x], replace);
                count++;
            }
        }
    }

    return count;
}

char*** AnsiBuilder_scale(char*** array, double scale) {
    if (array == 0) return 0;
    if (scale <= 0) return 0;

    int width = getAnsiArrayWidth(array);
    int height = getAnsiArrayHeight(array);

    if (width <= 0 || height <= 0) return 0;

    char*** scaled = (char***) malloc(height * scale * sizeof(char**));
    if (scaled == 0) return 0;

    for (int i = 0; i < height * scale; i++) {
        scaled[i] = (char**) malloc(width * scale * sizeof(char*));

        if (scaled[i] == 0) {
            for (int j = 0; j < i; j++) free(scaled[j]);
            free(scaled);
            return 0;
        }
    }

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            for (int sy = 0; sy < scale; sy++)
                for (int sx = 0; sx < scale; sx++)
                    strcpy(scaled[(int) (y * scale + sy)][(int) (x * scale + sx)], array[y][x]);

    // Free the original array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);

    return scaled;
}

#pragma endregion