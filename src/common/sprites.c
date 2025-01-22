#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/sprites.h"
#include "cmdfx/canvas.h"

CmdFX_Sprite** sprites = 0;
int spriteCount = 0;

CmdFX_Sprite** Canvas_getDrawnSprites() {
    return sprites;
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

    _getSpriteDimensions(data, &sprite->width, &sprite->height);
    sprite->data = data;
    sprite->ansi = ansi;

    return sprite;
}

void Sprite_free(CmdFX_Sprite* sprite) {
    if (sprite == 0) return;
    if (sprite->id != 0) Sprite_remove(sprite);

    for (int i = 0; i < sprite->height; i++) {
        if (sprite->data[i] == 0) continue;

        free(sprite->data[i]);

        if (sprite->ansi != 0) {
            char** ansi = sprite->ansi[i];
            if (ansi == 0) continue;

            char* line = ansi[0];
            while (line != 0) {
                free(line);
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

    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    for (int i = 0; i < sprite->height; i++) {
        char* line = sprite->data[i];
        if (line == 0) continue;

        for (int j = 0; j < sprite->width; j++) {
            char c = line[j];
            if (c == 0 || c == ' ') continue;

            int x = sprite->x + j;
            int y = sprite->y + i;

            // Check Z-Index Collision
            int skip = 0;
            for (int k = 0; k < spriteCount; k++) {
                CmdFX_Sprite* other = sprites[k];
                if (other->id == 0) continue;
                if (other->id == sprite->id) continue;

                if (other->z > sprite->z)
                    if (Sprite_isColliding(sprite, other)) skip = 1;
            }
            if (skip) continue;

            int hasAnsi = sprite->ansi != 0;
            Canvas_setCursor(x, y);
            if (hasAnsi) {
                char* ansi = sprite->ansi[i][j];
                Canvas_setAnsi(x, y, ansi);
            }
            putchar(c);
            if (hasAnsi) Canvas_resetFormat();
        }
    }

    Canvas_setCursor(oldX, oldY);
}

int Sprite_draw(int x, int y, CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 1 || y < 1) return 0;

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
        sprites = malloc(sizeof(CmdFX_Sprite*));
        if (!sprites) {
            free(sprite);
            return 0;
        }

        sprite->id = 1;
        sprites[0] = sprite;
        sprites[1] = 0;
    } else {
        CmdFX_Sprite** temp = realloc(sprites, sizeof(CmdFX_Sprite*) * (spriteCount + 2));
        if (!temp) {
            free(sprite);
            return 0;
        }
        sprites = temp;

        sprite->id = ++spriteCount;
        sprites[spriteCount] = sprite;
        sprites[spriteCount + 1] = 0;
    }

    return 1;
}

void Sprite_remove0(CmdFX_Sprite* sprite) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            Canvas_setCursor(sprite->x + j, sprite->y + i);
            Canvas_resetFormat();
            putchar(' ');
        }
    }

    Canvas_setCursor(oldX, oldY);
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

    CmdFX_Sprite** temp = realloc(sprites, sizeof(CmdFX_Sprite*) * (spriteCount - 1));
    if (!temp) return;
    sprites = temp;

    spriteCount--;
}

// Utility Methods - Sprite Builder

int Sprite_setChar(CmdFX_Sprite* sprite, int x, int y, char c) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    sprite->data[y][x] = c;
    return 1;
}

int Sprite_setAnsi(CmdFX_Sprite* sprite, int x, int y, const char* ansi) {
    if (sprite == 0) return 0;
    if (sprite->ansi == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    char* ansi0 = malloc(strlen(ansi) + 6);
    sprintf(ansi0, "\033[%sm", ansi);

    sprite->ansi[y][x] = ansi0;
    return 1;
}

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
                for (int j = 0; j <= i; j++) {
                    free(newData[j]);
                }
                free(newData);

                for (int j = 0; j < i; j++) {
                    free(newAnsi[j]);
                    newData[i][j] = (sprite->data[i] != 0) ? sprite->data[i][j] : padding;
                }

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
    if (sprite->id == 0) return;
    if (x < 1 || y < 1) return;

    Sprite_remove0(sprite);
    sprite->x = x;
    sprite->y = y;
    Sprite_draw0(sprite);
}

void Sprite_moveBy(CmdFX_Sprite* sprite, int dx, int dy) {
    if (sprite->id == 0) return;
    Sprite_moveTo(sprite, sprite->x + dx, sprite->y + dy);
}

// Utility Methods - Collisions

CmdFX_Sprite** Sprite_getCollidingSprites(CmdFX_Sprite* sprite) {
    if (sprite->id == 0) return 0;

    int collidingCount = 0;
    int allocated = 4;

    CmdFX_Sprite** colliding = malloc(sizeof(CmdFX_Sprite*) * allocated);
    if (!colliding) return 0;

    for (int i = 0; i < spriteCount; i++) {
        CmdFX_Sprite* other = sprites[i];
        if (other->id == 0) continue;
        if (other->id == sprite->id) continue;

        if (Sprite_isColliding(sprite, other)) {
            if (collidingCount >= allocated) {
                allocated *= 2;
                CmdFX_Sprite** temp = realloc(colliding, sizeof(CmdFX_Sprite*) * allocated);
                if (!temp) {
                    free(colliding);
                    return 0;
                }
                colliding = temp;
            }

            colliding[collidingCount++] = other;
        }
    }

    colliding[collidingCount] = 0;
    return colliding;
}

int Sprite_isColliding(CmdFX_Sprite* sprite1, CmdFX_Sprite* sprite2) {
    if (sprite1->id == 0 || sprite2->id == 0) return 0;
    if (sprite1->id == sprite2->id) return 0;

    return sprite1->x < sprite2->x + sprite2->width &&
        sprite1->x + sprite1->width > sprite2->x &&
        sprite1->y < sprite2->y + sprite2->height &&
        sprite1->y + sprite1->height > sprite2->y;
}