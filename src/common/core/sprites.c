#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/canvas.h"
#include "cmdfx/core/util.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/mass.h"

CmdFX_Sprite** sprites = 0;
int spriteCount = 0;

CmdFX_Sprite** Canvas_getDrawnSprites() {
    return sprites;
}

int Canvas_getDrawnSpritesCount() {
    return spriteCount;
}

CmdFX_Sprite* Canvas_getSpriteAt(int x, int y) {
    CmdFX_Sprite** matching = malloc(sizeof(CmdFX_Sprite*) * spriteCount);

    for (int i = 0; i < spriteCount; i++) {
        CmdFX_Sprite* sprite = sprites[i];
        if (sprite->id == 0) continue;

        if (x >= sprite->x && x < sprite->x + sprite->width)
            if (y >= sprite->y && y < sprite->y + sprite->height)
                matching[i] = sprite;
    }

    CmdFX_Sprite* sprite = 0;
    for (int i = 0; i < spriteCount; i++) {
        if (matching[i] == 0) continue;
        if (sprite == 0) {
            sprite = matching[i];
            continue;
        }

        if (matching[i]->z > sprite->z) sprite = matching[i];
    }

    free(matching);
    return sprite;
}

void _getSpriteDimensions(char** data, int* width, int* height) {
    *width = 0;
    *height = 0;

    if (data == 0) return;

    for (int i = 0; data[i] != 0; i++) {
        (*height)++;
        int len = strlen(data[i]);
        if (len > *width) {
            *width = len;
        }
    }
}

CmdFX_Sprite* Sprite_create(char** data, char*** ansi, int z) {
    CmdFX_Sprite* sprite = malloc(sizeof(CmdFX_Sprite));
    if (sprite == 0) return 0;

    sprite->x = -1;
    sprite->y = -1;
    sprite->z = z;
    sprite->id = 0;
    sprite->uid = 0;

    _getSpriteDimensions(data, &sprite->width, &sprite->height);
    sprite->data = data;
    sprite->ansi = ansi;

    return sprite;
}

void Sprite_free(CmdFX_Sprite* sprite) {
    if (sprite == 0) return;
    if (sprite->id > 0) Sprite_remove(sprite);

    for (int i = 0; i < sprite->height; i++) {
        if (sprite->data[i] == 0) continue;
        free(sprite->data[i]);

        if (sprite->ansi != 0) {
            char** ansi = sprite->ansi[i];
            if (ansi == 0) continue;

            int line = 0;
            while (line != 0) {
                free(ansi[line]);
                line++;
            }
            free(ansi);
        }
    }

    free(sprite->data);
    if (sprite->ansi != 0) free(sprite->ansi);

    free(sprite);
}

void Sprite_draw0(CmdFX_Sprite* sprite) {
    if (sprite->data == 0) return;

    int hasAnsi = sprite->ansi != 0;
    for (int i = 0; i < sprite->height; i++) {
        char* line = sprite->data[i];
        if (line == 0) continue;

        for (int j = 0; j < sprite->width; j++) {
            char c = line[j];
            if (c == 0 || c == ' ') continue;

            int x = sprite->x + j;
            int y = sprite->y + i;

            // Check Z-Index Collision
            if (!Sprite_isOnTop(sprite, x, y)) continue;

            Canvas_setCursor(x, y);
            if (hasAnsi) {
                char* ansi = sprite->ansi[i][j];
                if (ansi != 0)
                    Canvas_setAnsiCurrent(ansi);
            }
            putchar(c);
            if (hasAnsi) Canvas_resetFormat();
        }
    }
    fflush(stdout);
}

int Sprite_draw(int x, int y, CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0) return 0;

    sprite->x = x;
    sprite->y = y;

    if (sprite->id != 0) {
        CmdFX_Sprite* old = sprites[sprite->id - 1];

        // Redraw Sprite
        *old = *sprite;
        Sprite_draw0(old);
        return 1;
    }

    // Draw Sprite
    Sprite_draw0(sprite);

    // Add Sprite to List
    if (sprites == 0) {
        sprites = malloc(sizeof(CmdFX_Sprite*) * 2);
        if (!sprites) return 0;
    } else {
        CmdFX_Sprite** temp = realloc(sprites, sizeof(CmdFX_Sprite*) * (spriteCount + 2));
        if (!temp) return 0;

        sprites = temp;
    }

    sprites[spriteCount] = sprite;
    sprites[spriteCount + 1] = 0;
    sprite->id = ++spriteCount;
    sprite->uid = spriteCount;

    return 1;
}

void Sprite_remove0(CmdFX_Sprite* sprite) {
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            int x = sprite->x + j;
            int y = sprite->y + i;

            if (Sprite_isOnBottom(sprite, x, y)) {
                Canvas_setCursor(x, y);
                Canvas_resetFormat();
                putchar(' ');
            }
        }
    }
}

void Sprite_remove(CmdFX_Sprite* sprite) {
    if (sprite->id == 0) return;

    Sprite_remove0(sprite);
    
    int index = sprite->id - 1;
    if (sprites[index] == 0) return;

    for (int i = index; i < spriteCount - 1; i++) {
        sprites[i] = sprites[i + 1];
        sprites[i]->id--;
    }
    sprites[spriteCount - 1] = 0;

    CmdFX_Sprite** temp = realloc(sprites, sizeof(CmdFX_Sprite*) * spriteCount);
    if (temp)
        sprites = temp;
    else
        sprites[spriteCount] = 0;

    spriteCount--;

    // Reset Physics declarations
    Sprite_removeAllForces(sprite);
    Sprite_resetMass(sprite);
    Sprite_resetFrictionCoefficient(sprite);
}

// Utility Methods - Sprite Builder

int Sprite_setData(CmdFX_Sprite* sprite, char** data) {
    if (sprite == 0) return 0;
    if (data == 0) return 0;

    int width = 0;
    int height = 0;
    _getSpriteDimensions(data, &width, &height);

    if (width == 0 || height == 0) return 0;
    int changedDimensions = width != sprite->width || height != sprite->height;

    for (int i = 0; i < sprite->height; i++) {
        if (sprite->data[i] == 0) continue;
        free(sprite->data[i]);
    }
    free(sprite->data);

    sprite->width = width;
    sprite->height = height;
    sprite->data = data;

    if (sprite->ansi != 0 && changedDimensions) {
        char*** ansi = realloc(sprite->ansi, sizeof(char**) * height);
        if (ansi == 0) return 0;

        for (int i = 0; i < height; i++) {
            if (ansi[i] == 0) {
                ansi[i] = calloc(width, sizeof(char*));
                if (ansi[i] == 0) return 0;
            } else {
                char** ansiLine = ansi[i];
                for (int j = 0; j < width; j++) {
                    if (j >= sprite->width) {
                        ansiLine[j] = 0;
                        continue;
                    }
                    if (ansiLine[j] != 0) free(ansiLine[j]);
                }
                ansi[i] = realloc(ansiLine, sizeof(char*) * width);
                if (ansi[i] == 0) return 0;
            }
        }
    }

    return 1;
}

int Sprite_setChar(CmdFX_Sprite* sprite, int x, int y, char c) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    sprite->data[y][x] = c;

    // Redraw Character if Drawn
    if (sprite->id != 0)
        Canvas_setChar(sprite->x + x, sprite->y + y, c);

    return 1;
}

int Sprite_fillChar(CmdFX_Sprite* sprite, int x, int y, char c, int width, int height) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    for (int i = y; i < y + height; i++) {
        if (i >= sprite->height) break;
        for (int j = x; j < x + width; j++) {
            if (j >= sprite->width) break;
            sprite->data[i][j] = c;
        }
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_fillCharEmpty(CmdFX_Sprite* sprite, int x, int y, char c, int width, int height) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    for (int i = y; i < y + height; i++) {
        if (i >= sprite->height) break;
        for (int j = x; j < x + width; j++) {
            if (j >= sprite->width) break;
            char data = sprite->data[i][j];
            if (data == 0 || data == ' ') sprite->data[i][j] = c;
        }
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_fillCharAll(CmdFX_Sprite* sprite, char c) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            sprite->data[i][j] = c;
        }
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_fillCharAllEmpty(CmdFX_Sprite* sprite, char c) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            char data = sprite->data[i][j];
            if (data == 0 || data == ' ') sprite->data[i][j] = c;
        }
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_setAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi) {
    if (sprite == 0) return 0;
    if (sprite->ansi == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    char* ansi0 = malloc(strlen(ansi) + 1);
    if (ansi0 == 0) return 0;

    strcpy(ansi0, ansi);

    if (sprite->ansi[y][x] != 0) free(sprite->ansi[y][x]);
    sprite->ansi[y][x] = ansi0;
    
    return 1;
}

int Sprite_appendAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi) {
    if (sprite == 0) return 0;
    if (ansi == 0) return 0;
    if (sprite->ansi == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    if (sprite->ansi[y][x] == 0) {
        char* ansi0 = malloc(strlen(ansi) + 1);
        if (ansi0 == 0) return 0;

        strcpy(ansi0, ansi);

        sprite->ansi[y][x] = ansi0;
    } else {
        int size = strlen(sprite->ansi[y][x]) + strlen(ansi) + 1;
        char* ansi0 = realloc(sprite->ansi[y][x], size);
        if (ansi0 == 0) return 0;

        sprite->ansi[y][x] = ansi0;
        strcat(sprite->ansi[y][x], ansi);
    }

    return 1;
}

int Sprite_fillAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi, int width, int height) {
    if (sprite == 0) return 0;
    if (sprite->ansi == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    for (int i = y; i < y + height; i++) {
        if (i >= sprite->height) break;
        for (int j = x; j < x + width; j++) {
            if (j >= sprite->width) break;
            char* ansi0 = malloc(strlen(ansi) + 1);
            sprintf(ansi0, "%s", ansi);

            if (sprite->ansi[i][j] != 0) free(sprite->ansi[i][j]);
            sprite->ansi[i][j] = ansi0;
        }
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_setAnsiAll(CmdFX_Sprite* sprite, char* ansi) {
    if (sprite == 0) return 0;
    if (sprite->ansi == 0) return 0;

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            char* ansi0 = malloc(strlen(ansi) + 1);
            sprintf(ansi0, "%s", ansi);

            if (sprite->ansi[i][j] != 0) free(sprite->ansi[i][j]);
            sprite->ansi[i][j] = ansi0;
        }
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_appendAnsiAll(CmdFX_Sprite* sprite, char* ansi) {
    if (sprite == 0) return 0;
    if (sprite->ansi == 0) return 0;

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            Sprite_appendAnsi(sprite, j, i, ansi);
        }
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

// Utility Methods - Sprite Creation

CmdFX_Sprite* Sprite_createFilled(int width, int height, char c, char* ansi, int z) {
    if (width <= 0 || height <= 0) return 0;

    char** data = malloc(sizeof(char*) * (height + 1));
    if (data == 0) return 0;

    for (int i = 0; i < height; i++) {
        data[i] = malloc(sizeof(char) * (width + 1));
        if (data[i] == 0) {
            for (int j = 0; j < i; j++) free(data[j]);
            free(data);
            return 0;
        }

        for (int j = 0; j < width; j++) data[i][j] = c;
        data[i][width] = 0;
    }
    data[height] = 0;

    char*** ansiData = calloc(height + 1, sizeof(char**));
    for (int i = 0; i < height; i++) {
        ansiData[i] = calloc(width + 1, sizeof(char*));
        for (int j = 0; j < width; j++) {
            ansiData[i][j] = 0;
        }
    }

    CmdFX_Sprite* sprite = Sprite_create(data, ansiData, z);
    Sprite_fillCharAll(sprite, c);
    if (ansi != 0) 
        Sprite_setAnsiAll(sprite, ansi);

    return sprite;
}

CmdFX_Sprite* Sprite_loadFromFile(const char* path, int z) {
    FILE* file = fopen(path, "r");
    if (file == 0) return 0;

    char** data = 0;
    int width = 0, height = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = 0;

        if (data == 0) {
            data = malloc(sizeof(char*) * 2);
            if (data == 0) break;
            data[0] = malloc(sizeof(char) * (strlen(line) + 1));
            if (data[0] == 0) {
                free(data);
                break;
            }
            strcpy(data[0], line);
            data[1] = 0;
            width = strlen(line);
            height = 1;
        } else {
            char** newData = realloc(data, sizeof(char*) * (height + 2));
            if (newData == 0) break;
            data = newData;
            data[height] = malloc(sizeof(char) * (strlen(line) + 1));
            if (data[height] == 0) break;
            strcpy(data[height], line);
            data[height + 1] = 0;
            height++;
        }
    }

    fclose(file);
    if (data == 0) return 0;

    CmdFX_Sprite* sprite = Sprite_create(data, 0, z);
    if (sprite == 0) {
        for (int i = 0; i < height; i++) free(data[i]);
        free(data);
    }

    return sprite;
}

// Utility Methods - Sizing

int Sprite_resize0(CmdFX_Sprite* sprite, int width, int height, char padding) {
    if (sprite->data == 0) {
        sprite->data = malloc(sizeof(char*) * sprite->height);
        if (sprite->data == 0) return 0;
        for (int i = 0; i < sprite->height; i++) {
            sprite->data[i] = malloc(sizeof(char) * sprite->width);
            if (sprite->data[i] == 0) return 0;
            memset(sprite->data[i], padding, sprite->width);
        }
    }

    char** newData = malloc(sizeof(char*) * height);
    if (newData == 0) return 0;

    char*** newAnsi = 0;
    if (sprite->ansi != 0) {
        newAnsi = malloc(sizeof(char**) * height);
        if (newAnsi == 0) {
            free(newData);
            return 0;
        }
    }

    for (int i = 0; i < height; i++) {
        newData[i] = malloc(sizeof(char) * width);

        // Fail check
        if (newData[i] == 0) {
            for (int j = 0; j < i; j++) free(newData[j]);
            free(newData);
            if (newAnsi != 0) {
                for (int j = 0; j < i; j++) free(newAnsi[j]);
                free(newAnsi);
            }
            return 0;
        }

        memset(newData[i], padding, width);

        if (newAnsi != 0) {
            newAnsi[i] = malloc(sizeof(char*) * width);

            // Fail check
            if (newAnsi[i] == 0) {
                for (int j = 0; j <= i; j++) free(newData[j]);
                free(newData);

                for (int j = 0; j < i; j++) free(newAnsi[j]);
                free(newAnsi);
            }

            for (int j = 0; j < width; j++) newAnsi[i][j] = 0;
        }
    }

    for (int i = 0; i < sprite->height && i < height; i++) {
        for (int j = 0; j < sprite->width && j < width; j++) {
            newData[i][j] = sprite->data[i][j];
            if (newAnsi != 0 && sprite->ansi != 0) {
                newAnsi[i][j] = sprite->ansi[i][j];
            }
        }
    }

    for (int i = 0; i < sprite->height; i++) {
        free(sprite->data[i]);
        if (sprite->ansi != 0) {
            for (int j = 0; j < sprite->width; j++) {
                if (sprite->ansi[i][j] != 0) free(sprite->ansi[i][j]);
            }
            free(sprite->ansi[i]);
        }
    }

    free(sprite->data);
    if (sprite->ansi != 0) free(sprite->ansi);

    sprite->data = newData;
    sprite->ansi = newAnsi;
    sprite->width = width;
    sprite->height = height;

    return 1;
}

int Sprite_resize(CmdFX_Sprite* sprite, int width, int height) {
    return Sprite_resizeWithPadding(sprite, width, height, ' ');
}

int Sprite_resizeWithPadding(CmdFX_Sprite* sprite, int width, int height, char padding) {
    if (sprite == 0) return 0;
    if (width < 1 || height < 1) return 0;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (!Sprite_resize0(sprite, width, height, padding)) {
        Sprite_draw0(sprite); // Redraw, resize failed
        return 0;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_center0(CmdFX_Sprite* sprite) {
    int left = sprite->width, right = 0, top = sprite->height, bottom = 0;

    // Find the bounding box of non-whitespace characters
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            if (sprite->data[i][j] != ' ') {
                if (j < left) left = j;
                if (j > right) right = j;
                if (i < top) top = i;
                if (i > bottom) bottom = i;
            }
        }
    }

    int newWidth = right - left + 1;
    int newHeight = bottom - top + 1;
    int offsetX = (sprite->width - newWidth) / 2;
    int offsetY = (sprite->height - newHeight) / 2;

    // Move the contents of data and ansi toward the center
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            sprite->data[offsetY + i][offsetX + j] = sprite->data[top + i][left + j];
            if (sprite->ansi != 0) {
                sprite->ansi[offsetY + i][offsetX + j] = sprite->ansi[top + i][left + j];
            }
        }
    }

    // Fill the remaining area with whitespace and reset ansi
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            if (i < offsetY || i >= offsetY + newHeight || j < offsetX || j >= offsetX + newWidth) {
                sprite->data[i][j] = ' ';
                if (sprite->ansi != 0) {
                    sprite->ansi[i][j] = 0;
                }
            }
        }
    }

    return 1;
}

int Sprite_center(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (!Sprite_center0(sprite)) {
        Sprite_draw0(sprite);  // Redraw, center failed
        return 0;
    }

    if (!Sprite_center0(sprite)) {
        Sprite_draw0(sprite);  // Redraw, center failed
        return 0;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_resizeAndCenter(CmdFX_Sprite* sprite, int width, int height) {
    if (sprite == 0) return 0;
    if (width < 1 || height < 1) return 0;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (!Sprite_resize0(sprite, width, height, ' ')) {
        Sprite_draw0(sprite); // Redraw, resize failed
        return 0;
    }

    if (!Sprite_center0(sprite)) {
        Sprite_draw0(sprite); // Redraw, center failed
        return 0;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

// Utility Methods - Movement

void Sprite_moveTo(CmdFX_Sprite* sprite, int x, int y) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;

    Sprite_remove0(sprite);
    sprite->x = x;
    sprite->y = y;
    Sprite_draw0(sprite);
}

void Sprite_moveBy(CmdFX_Sprite* sprite, int dx, int dy) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;

    Sprite_moveTo(sprite, sprite->x + dx, sprite->y + dy);
}

// Utility Methods - Collisions

CmdFX_Sprite** Sprite_getCollidingSprites(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (spriteCount < 2) return 0;

    int collidingCount = 0;
    int allocated = 4;

    CmdFX_Sprite** colliding = calloc(allocated + 1, sizeof(CmdFX_Sprite*));
    if (!colliding) return 0;

    for (int i = 0; i < spriteCount; i++) {
        CmdFX_Sprite* other = sprites[i];
        if (other->id == 0) continue;
        if (other->id == sprite->id) continue;

        if (Sprite_isColliding(sprite, other)) {
            if (collidingCount >= allocated) {
                allocated *= 2;
                CmdFX_Sprite** temp = realloc(colliding, sizeof(CmdFX_Sprite*) * (allocated + 1));
                if (!temp) {
                    free(colliding);
                    return 0;
                }
                colliding = temp;

                // realloc a calloc
                for (int j = collidingCount; j < allocated + 1; j++) colliding[j] = 0;
            }

            colliding[collidingCount++] = other;
        }
    }

    colliding[collidingCount] = 0;
    return colliding;
}

int Sprite_isColliding(CmdFX_Sprite* sprite1, CmdFX_Sprite* sprite2) {
    if (sprite1 == 0) return 0;
    if (sprite2 == 0) return 0;

    if (sprite1->id == 0 || sprite2->id == 0) return 0;
    if (sprite1->id == sprite2->id) return 0;

    return sprite1->x < sprite2->x + sprite2->width &&
        sprite1->x + sprite1->width > sprite2->x &&
        sprite1->y < sprite2->y + sprite2->height &&
        sprite1->y + sprite1->height > sprite2->y;
}

int Sprite_isOnTop(CmdFX_Sprite* sprite, int x, int y) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (x < sprite->x || y < sprite->y) return 0;
    if (x >= sprite->x + sprite->width || y >= sprite->y + sprite->height) return 0;

    CmdFX_Sprite** collisions = Sprite_getCollidingSprites(sprite);
    if (collisions == 0) return 1;

    for (int i = 0; collisions[i] != 0; i++) {
        CmdFX_Sprite* other = collisions[i];
        if (x < other-> x || y < other->y) continue;
        if (x >= other->x + other->width || y >= other->y + other->height) continue;

        if (other->z > sprite->z) {
            free(collisions);
            return 0;
        }
    }

    return 1;
}

int Sprite_isOnBottom(CmdFX_Sprite* sprite, int x, int y) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (x < sprite->x || y < sprite->y) return 0;
    if (x >= sprite->x + sprite->width || y >= sprite->y + sprite->height) return 0;

    CmdFX_Sprite** collisions = Sprite_getCollidingSprites(sprite);
    if (collisions == 0) return 1;

    for (int i = 0; collisions[i] != 0; i++) {
        CmdFX_Sprite* other = collisions[i];
        if (x < other-> x || y < other->y) continue;
        if (x >= other->x + other->width || y >= other->y + other->height) continue;

        if (other->z < sprite->z) {
            free(collisions);
            return 0;
        }
    }

    return 1;
}

// Utility Methods - Color

int Sprite_setForeground(CmdFX_Sprite* sprite, int x, int y, int rgb) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    sprintf(ansi, "\033[38;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);

    return Sprite_appendAnsi(sprite, x, y, ansi);
}

int Sprite_setForeground256(CmdFX_Sprite* sprite, int x, int y, int color) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    sprintf(ansi, "\033[38;5;%dm", color);

    return Sprite_appendAnsi(sprite, x, y, ansi);
}

int Sprite_setForegroundAll(CmdFX_Sprite* sprite, int rgb) {
    if (sprite == 0) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    sprintf(ansi, "\033[38;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);

    return Sprite_appendAnsiAll(sprite, ansi);
}

int Sprite_setForegroundAll256(CmdFX_Sprite* sprite, int color) {
    if (sprite == 0) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    sprintf(ansi, "\033[38;5;%dm", color);

    return Sprite_appendAnsiAll(sprite, ansi);
}

int Sprite_setBackground(CmdFX_Sprite* sprite, int x, int y, int rgb) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    sprintf(ansi, "\033[48;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);

    int res = Sprite_appendAnsi(sprite, x, y, ansi);
    free(ansi);

    return res;
}

int Sprite_setBackground256(CmdFX_Sprite* sprite, int x, int y, int color) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    sprintf(ansi, "\033[48;5;%dm", color);

    int res = Sprite_appendAnsi(sprite, x, y, ansi);
    free(ansi);

    return res;
}

int Sprite_setBackgroundAll(CmdFX_Sprite* sprite, int rgb) {
    if (sprite == 0) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    sprintf(ansi, "\033[48;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);

    int res = Sprite_appendAnsiAll(sprite, ansi);
    free(ansi);

    return res;
}

int Sprite_setBackgroundAll256(CmdFX_Sprite* sprite, int color) {
    if (sprite == 0) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    if (ansi == 0) return 0;

    snprintf(ansi, 14, "\033[48;5;%dm", color);

    int res = Sprite_appendAnsiAll(sprite, ansi);
    free(ansi);

    return res;
}

// Utility Methods - Color Gradient

void _freeANSI(char*** ansi, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) free(ansi[i][j]);
        free(ansi[i]);
    }
    free(ansi);
}

int Sprite_setGradient0(CmdFX_Sprite* sprite, int prefix, int x, int y, int width, int height, enum CmdFX_GradientDirection direction, int numColors, va_list* args) {
    int* colors = (int*) malloc(sizeof(int) * (numColors + 1));
    if (colors == 0) return 0;

    for (int i = 0; i < numColors; i++) {
        colors[i] = va_arg(*args, int);
    }

    char*** gradient = AnsiBuilder_create(width, height);
    if (gradient == 0) {
        free(colors);
        return 0;
    }
    if (prefix == 38)
        AnsiBuilder_multiGradientForegroundFull(gradient, numColors, colors, direction);
    else
        AnsiBuilder_multiGradientBackgroundFull(gradient, numColors, colors, direction);

    for (int j = 0; j < height; j++) {
        int cy = y + j;
        if (cy < 0 || cy >= sprite->height) continue;

        for (int i = 0; i < width; i++) {
            int cx = x + i;
            if (cx < 0 || cx >= sprite->width) continue;

            free(sprite->ansi[cy][cx]);
            sprite->ansi[cy][cx] = gradient[j][i];
        }
    }

    free(colors);
    _freeANSI(gradient, width, height);

    if (sprite->id != 0) {
        Sprite_remove0(sprite);
        Sprite_draw0(sprite);
    }

    return 1;
}

int Sprite_setForegroundGradient(CmdFX_Sprite* sprite, int x, int y, int width, int height, enum CmdFX_GradientDirection direction, int numColors, ...) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(sprite, 38, x, y, width, height, direction, numColors, &args);
    va_end(args);

    return res;
}

int Sprite_setForegroundGradientAll(CmdFX_Sprite* sprite, enum CmdFX_GradientDirection direction, int numColors, ...) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(sprite, 38, 0, 0, sprite->width, sprite->height, direction, numColors, &args);
    va_end(args);

    return res;
}

int Sprite_setBackgroundGradient(CmdFX_Sprite* sprite, int x, int y, int width, int height, enum CmdFX_GradientDirection direction, int numColors, ...) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(sprite, 48, x, y, width, height, direction, numColors, &args);
    va_end(args);

    return res;
}

int Sprite_setBackgroundGradientAll(CmdFX_Sprite* sprite, enum CmdFX_GradientDirection direction, int numColors, ...) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(sprite, 48, 0, 0, sprite->width, sprite->height, direction, numColors, &args);
    va_end(args);

    return res;
}

// Utility Methods - Transformations

int Sprite_rotate(CmdFX_Sprite* sprite, double radians) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (sprite->data != 0)
        CharBuilder_rotate(sprite->data, radians);
    
    if (sprite->ansi != 0)
        AnsiBuilder_rotate(sprite->ansi, radians);
    
    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 0;
}

double Sprite_getRotationAngle(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0.0;
    if (sprite->data == 0) return 0.0;

    return CharBuilder_getRotationAngle(sprite->data);
}

int Sprite_hFlip(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (sprite->data != 0)
        CharBuilder_hFlip(sprite->data);
    
    if (sprite->ansi != 0)
        AnsiBuilder_hFlip(sprite->ansi);
    
    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 0;
}

int Sprite_vFlip(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (sprite->data != 0)
        CharBuilder_vFlip(sprite->data);
    
    if (sprite->ansi != 0)
        AnsiBuilder_vFlip(sprite->ansi);
    
    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 0;
}

int Sprite_scale(CmdFX_Sprite* sprite, double scale) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (sprite->data != 0) {
        char** newData = CharBuilder_scale(sprite->data, scale);
        sprite->data = newData;
    }

    if (sprite->ansi != 0) {
        char*** newAnsi = AnsiBuilder_scale(sprite->ansi, scale);
        sprite->ansi = newAnsi;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 0;
}

int Sprite_transpose(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) Sprite_remove0(sprite);

    if (sprite->data != 0) {
        char** newData = CharBuilder_transpose(sprite->data);
        sprite->data = newData;
    }

    if (sprite->ansi != 0) {
        char*** newAnsi = AnsiBuilder_transpose(sprite->ansi);
        sprite->ansi = newAnsi;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 0;
}