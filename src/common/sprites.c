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

CmdFX_Sprite* Sprite_create(int x, int y, int width, int height, char** data, char*** ansi, int z) {
    if (data == 0) return 0;
    if (x < 1 || y < 1) return 0;
    if (width < 1 || height < 1) return 0;

    int rows = sizeof(data) / sizeof(data[0]);
    if (rows != height) return 0;

    for (int i = 0; i < rows; i++) {
        if (strlen(data[i]) != width) return 0;
    }

    CmdFX_Sprite* sprite = malloc(sizeof(CmdFX_Sprite));
    if (sprite == 0) return 0;

    sprite->x = x;
    sprite->y = y;
    sprite->width = width;
    sprite->height = height;
    sprite->data = data;
    sprite->ansi = ansi;
    sprite->z = z;
    sprite->id = 0;

    return sprite;
}

void Sprite_draw0(CmdFX_Sprite* sprite) {
    int oldX = Canvas_getCursorX();
    int oldY = Canvas_getCursorY();

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            char* ansi = sprite->ansi[i][j];

            Canvas_setCursor(sprite->x + j, sprite->y + i);
            Canvas_setAnsi(sprite->x + j, sprite->y + i, ansi);
            putchar(sprite->data[i][j]);
            Canvas_resetFormat();
        }
    }

    Canvas_setCursor(oldX, oldY);
}

void Sprite_draw(CmdFX_Sprite* sprite) {
    if (sprite->id != 0) {
        CmdFX_Sprite* old = sprites[sprite->id - 1];

        // Redraw Sprite
        *old = *sprite;
        Sprite_draw0(old);
        return;
    }

    // Draw Sprite
    Sprite_draw0(sprite);

    // Add Sprite to List
    if (sprites == 0) {
        sprites = malloc(sizeof(CmdFX_Sprite*));
        if (!sprites) {
            free(sprite);
            return;
        }

        sprite->id = 1;
        sprites[0] = sprite;
        sprites[1] = 0;
    } else {
        CmdFX_Sprite** temp = realloc(sprites, sizeof(CmdFX_Sprite*) * (spriteCount + 2));
        if (!temp) {
            free(sprite);
            return;
        }
        sprites = temp;

        sprite->id = ++spriteCount;
        sprites[spriteCount] = sprite;
        sprites[spriteCount + 1] = 0;
    }
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