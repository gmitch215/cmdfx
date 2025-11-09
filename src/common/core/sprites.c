#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "cmdfx/core/builder.h"
#include "cmdfx/core/canvas.h"
#include "cmdfx/core/costumes.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/physics/force.h"
#include "cmdfx/physics/mass.h"
#include "cmdfx/physics/motion.h"

#define _SPRITE_DRAWN_MUTEX 0
static CmdFX_Sprite** _sprites = 0;
static int _spriteCount = 0;

#define _SPRITE_UID_MUTEX 1
static int _spriteUidCounter = 0;

static int* _takenUids = 0;

// Per-sprite locking utilities
#define _FIRST_SPRITE_MUTEX_ID 11

int _spriteMutexId(const CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    int range = MAX_INTERNAL_CMDFX_MUTEXES - _FIRST_SPRITE_MUTEX_ID;

    // Simple mix of uid to spread across mutex pool
    unsigned uid = (unsigned) sprite->uid;

    // Knuth multiplicative hash
    // Decorrelates integers
    int id =
        _FIRST_SPRITE_MUTEX_ID + (int) ((uid * 0x9e3779b9u) % (unsigned) range);
    return id;
}

void _lockSprite(const CmdFX_Sprite* sprite) {
    int id = _spriteMutexId(sprite);
    if (id >= 0) CmdFX_tryLockMutex(id);
}

void _unlockSprite(const CmdFX_Sprite* sprite) {
    int id = _spriteMutexId(sprite);
    if (id >= 0) CmdFX_tryUnlockMutex(id);
}

// Lock two sprites in a stable order (by uid) to avoid deadlocks when two
// threads act on two sprites.
void _lockSpritePair(const CmdFX_Sprite* a, const CmdFX_Sprite* b) {
    if (a == b) {
        _lockSprite(a);
        return;
    }
    if (a == 0 || b == 0) {
        if (a) _lockSprite(a);
        if (b) _lockSprite(b);
        return;
    }
    if (a->uid < b->uid) {
        _lockSprite(a);
        _lockSprite(b);
    }
    else {
        _lockSprite(b);
        _lockSprite(a);
    }
}

void _unlockSpritePair(const CmdFX_Sprite* a, const CmdFX_Sprite* b) {
    if (a == b) {
        _unlockSprite(a);
        return;
    }
    if (a == 0 || b == 0) {
        if (a) _unlockSprite(a);
        if (b) _unlockSprite(b);
        return;
    }
    if (a->uid < b->uid) {
        _unlockSprite(b);
        _unlockSprite(a);
    }
    else {
        _unlockSprite(a);
        _unlockSprite(b);
    }
}

CmdFX_Sprite** Canvas_getDrawnSprites() {
    return _sprites;
}

int Canvas_getDrawnSpritesCount() {
    return _spriteCount;
}

CmdFX_Sprite* Canvas_getSpriteAt(int x, int y) {
    if (x < 0 || y < 0) return 0;

    CmdFX_Sprite** matching = malloc(sizeof(CmdFX_Sprite*) * _spriteCount);

    for (int i = 0; i < _spriteCount; i++) {
        CmdFX_Sprite* sprite = _sprites[i];
        if (sprite->id == 0) continue;

        if (x >= sprite->x && x < sprite->x + sprite->width)
            if (y >= sprite->y && y < sprite->y + sprite->height)
                matching[i] = sprite;
    }

    CmdFX_Sprite* sprite = 0;
    for (int i = 0; i < _spriteCount; i++) {
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

    CmdFX_tryLockMutex(_SPRITE_UID_MUTEX);

    if (_takenUids == 0) {
        sprite->uid = ++_spriteUidCounter;

        _takenUids = calloc(1, sizeof(int));
        if (_takenUids == 0) {
            CmdFX_tryUnlockMutex(_SPRITE_UID_MUTEX);
            free(sprite);
            return 0;
        }
        _spriteUidCounter++;
        _takenUids[0] = sprite->uid;
    }
    else {
        bool* present = calloc(_spriteUidCounter + 1, sizeof(bool));

        int j = -1;
        for (int i = 0; i < _spriteUidCounter; i++) {
            int uid = _takenUids[i];
            if (uid >= 1 && uid <= _spriteUidCounter) {
                present[uid] = true;
            }
        }

        // Find the first missing number in the range [1, _spriteUidCounter]
        for (int i = 1; i <= _spriteUidCounter; i++) {
            if (!present[i]) {
                free(present);
                j = i;
                break;
            }
        }
        if (j == -1) {
            j = _spriteUidCounter + 1;
            free(present);
        }

        // j = lowest available UID
        sprite->uid = j;
        if (_spriteUidCounter < j)
            _spriteUidCounter =
                j; // if j is greater than the current counter, update counter

        int* temp = realloc(_takenUids, sizeof(int) * (_spriteUidCounter + 1));
        if (temp == 0) {
            CmdFX_tryUnlockMutex(_SPRITE_UID_MUTEX);
            free(sprite);
            return 0;
        }
        _takenUids = temp;
        _takenUids[_spriteUidCounter - 1] = sprite->uid;
        _spriteUidCounter++;
    }

    CmdFX_tryUnlockMutex(_SPRITE_UID_MUTEX);

    if (data != 0) _getSpriteDimensions(data, &sprite->width, &sprite->height);
    sprite->data = data;
    sprite->ansi = ansi;

    return sprite;
}

void Sprite_free(CmdFX_Sprite* sprite) {
    if (sprite == 0) return;
    if (sprite->id > 0) Sprite_remove(sprite);

    // Remove UID from Taken UIDs
    CmdFX_tryLockMutex(_SPRITE_UID_MUTEX);
    if (_takenUids != 0) {
        for (int i = 0; i < _spriteUidCounter; i++) {
            if (_takenUids[i] == sprite->uid) {
                _takenUids[i] = 0;
                break;
            }
        }

        int allZero = 1;
        for (int i = 0; i < _spriteUidCounter; i++) {
            if (_takenUids[i] != 0) {
                allZero = 0;
                break;
            }
        }

        if (_spriteUidCounter == 0 || allZero) {
            free(_takenUids);
            _takenUids = 0;
            _spriteUidCounter = 0;
        }
    }

    // Perform resize check
    if (_spriteUidCounter > 24) {
        int zeroCount = 0;
        for (int i = 0; i < _spriteUidCounter; i++) {
            if (_takenUids[i] == 0) zeroCount++;
        }

        if (zeroCount > 0) {
            int* temp = realloc(
                _takenUids, sizeof(int) * (_spriteUidCounter - zeroCount)
            );
            if (temp != 0) {
                _takenUids = temp;
                for (int i = 0; i < _spriteUidCounter; i++)
                    if (_takenUids[i] == 0) {
                        for (int j = i; j < _spriteUidCounter - 1; j++) {
                            _takenUids[j] = _takenUids[j + 1];
                        }
                        break;
                    }

                _spriteUidCounter -= zeroCount;
            }
        }
    }
    CmdFX_tryUnlockMutex(_SPRITE_UID_MUTEX);

    // Free Sprite Costumes and Data
    CmdFX_SpriteCostumes* costumes = Sprite_getCostumes(sprite);
    if (costumes != 0) {
        for (int i = 0; i < costumes->costumeCount; i++) {
            char** data = costumes->costumes[i];
            if (data != 0) {
                int height = getCharArrayHeight(data);
                for (int j = 0; j < height; j++) free(data[j]);
            }
            free(data);

            char*** ansi = costumes->ansiCostumes[i];
            if (ansi != 0) {
                int height = getStringArrayHeight(ansi);
                int width = getStringArrayWidth(ansi);
                for (int j = 0; j < height; j++) {
                    for (int k = 0; k < width; k++) free(ansi[j][k]);
                    free(ansi[j]);
                }
            }
            free(ansi);
        }
        free(costumes);
    }
    else {
        char** data = sprite->data;
        if (data != 0) {
            int height = getCharArrayHeight(data);
            for (int i = 0; i < height; i++) free(data[i]);
        }
        free(data);

        char*** ansi = sprite->ansi;
        if (ansi != 0) {
            int height = getStringArrayHeight(ansi);
            int width = getStringArrayWidth(ansi);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) free(ansi[i][j]);
                free(ansi[i]);
            }
        }
        free(ansi);
    }
    free(sprite);
}

#define _CANVAS_MUTEX 7

void Sprite_draw0(CmdFX_Sprite* sprite) {
    if (sprite->data == 0) return;

    if (sprite->x < 1 || sprite->y < 1) return;
    if (sprite->x + sprite->width > Canvas_getWidth() ||
        sprite->y + sprite->height > Canvas_getHeight())
        return;

    int hasAnsi = sprite->ansi != 0;
    for (int i = 0; i < sprite->height; i++) {
        char* line = sprite->data[i];
        if (line == 0) continue;

        for (int j = 0; j < sprite->width; j++) {
            char c = line[j];
            if (c == 0 || c == '\0' || c == ' ') continue;

            int x = sprite->x + j;
            int y = sprite->y + i;

            // Check Z-Index Collision
            if (!Sprite_isOnTop(sprite, x, y)) continue;

            Canvas_setCursor(x, y);
            if (hasAnsi) {
                char* ansi = sprite->ansi[i][j];
                if (ansi != 0) Canvas_setAnsiCurrent(ansi);
            }
            putchar(c);
            if (hasAnsi) Canvas_resetFormat();
        }
    }
    fflush(stdout);
}

#define _SPRITE_POSITION_MUTEX 2

int Sprite_draw(int x, int y, CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 1 || y < 1) return 0;

    int width = Canvas_getWidth();
    int height = Canvas_getHeight();
    if (width > 0 && height > 0) {
        if (x + sprite->width > width || y + sprite->height > height) return 0;
    }

    CmdFX_tryLockMutex(_SPRITE_POSITION_MUTEX);
    sprite->x = x;
    sprite->y = y;
    CmdFX_tryUnlockMutex(_SPRITE_POSITION_MUTEX);

    if (sprite->id != 0) {
        CmdFX_Sprite* old = _sprites[sprite->id - 1];

        // Redraw Sprite
        *old = *sprite;
        Sprite_draw0(old);
        return 1;
    }

    // Draw Sprite
    Sprite_draw0(sprite);

    // Add Sprite to List
    CmdFX_tryLockMutex(_SPRITE_DRAWN_MUTEX);

    if (_sprites == 0) {
        _sprites = malloc(sizeof(CmdFX_Sprite*) * 2);
        if (!_sprites) return 0;
    }
    else {
        CmdFX_Sprite** temp =
            realloc(_sprites, sizeof(CmdFX_Sprite*) * (_spriteCount + 2));
        if (!temp) return 0;

        _sprites = temp;
    }

    _sprites[_spriteCount] = sprite;
    _sprites[_spriteCount + 1] = 0;
    sprite->id = ++_spriteCount;

    CmdFX_tryUnlockMutex(_SPRITE_DRAWN_MUTEX);

    return 1;
}

void Sprite_remove0(CmdFX_Sprite* sprite) {
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            int x = sprite->x + j;
            int y = sprite->y + i;

            Canvas_setCursor(x, y);
            printf("\033[0m");
            putchar(' ');
        }
    }
    fflush(stdout);
}

void Sprite_remove(CmdFX_Sprite* sprite) {
    if (sprite->id == 0) return;

    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    Sprite_remove0(sprite);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);

    int index = sprite->id - 1;
    if (_sprites[index] == 0) return;

    CmdFX_tryLockMutex(_SPRITE_DRAWN_MUTEX);

    for (int i = index; i < _spriteCount - 1; i++) {
        _sprites[i] = _sprites[i + 1];
        _sprites[i]->id--;
    }
    _sprites[_spriteCount - 1] = 0;

    CmdFX_Sprite** temp =
        realloc(_sprites, sizeof(CmdFX_Sprite*) * _spriteCount);
    if (temp)
        _sprites = temp;
    else
        _sprites[_spriteCount] = 0;

    _spriteCount--;
    if (_spriteCount == 0) {
        free(_sprites);
        _sprites = 0;
    }

    sprite->id = 0;

    CmdFX_tryLockMutex(_SPRITE_POSITION_MUTEX);
    sprite->x = -1;
    sprite->y = -1;
    CmdFX_tryUnlockMutex(_SPRITE_POSITION_MUTEX);

    CmdFX_tryUnlockMutex(_SPRITE_DRAWN_MUTEX);

    // Reset Physics declarations
    Sprite_resetAllMotion(sprite);
    Sprite_removeAllForces(sprite);
    Sprite_resetMass(sprite);
    Sprite_resetFrictionCoefficient(sprite);
}

// Utility Methods - Sprite Builder

#define _SPRITE_DATA_MUTEX 3

int Sprite_setData(CmdFX_Sprite* sprite, char** data) {
    if (sprite == 0) return 0;
    if (data == 0) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);
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
            }
            else {
                char** ansiLine = ansi[i];
                for (int j = 0; j < width; j++) {
                    if (j >= sprite->width) {
                        ansiLine[j] = 0;
                        continue;
                    }
                    free(ansiLine[j]);
                }
                ansi[i] = realloc(ansiLine, sizeof(char*) * width);
                if (ansi[i] == 0) return 0;
            }
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);
    return 1;
}

int Sprite_setChar(CmdFX_Sprite* sprite, int x, int y, char c) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);
    sprite->data[y][x] = c;
    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    // Redraw Character if Drawn
    if (sprite->id != 0) Canvas_setChar(sprite->x + x, sprite->y + y, c);

    return 1;
}

int Sprite_fillChar(
    CmdFX_Sprite* sprite, int x, int y, int width, int height, char c
) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);

    for (int i = y; i < y + height; i++) {
        if (i >= sprite->height) break;
        for (int j = x; j < x + width; j++) {
            if (j >= sprite->width) break;
            sprite->data[i][j] = c;
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_fillCharEmpty(
    CmdFX_Sprite* sprite, int x, int y, int width, int height, char c
) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);

    for (int i = y; i < y + height; i++) {
        if (i >= sprite->height) break;
        for (int j = x; j < x + width; j++) {
            if (j >= sprite->width) break;
            char data = sprite->data[i][j];
            if (data == 0 || data == ' ') sprite->data[i][j] = c;
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_fillCharAll(CmdFX_Sprite* sprite, char c) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            sprite->data[i][j] = c;
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_fillCharAllEmpty(CmdFX_Sprite* sprite, char c) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            char data = sprite->data[i][j];
            if (data == 0 || data == ' ') sprite->data[i][j] = c;
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_setAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi) {
    if (sprite == 0) return 0;
    if (ansi == 0) return 0; // Add null check for ansi parameter
    if (sprite->ansi == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);

    char* ansi0 = malloc(strlen(ansi) + 1);
    if (ansi0 == 0) return 0;

    strcpy(ansi0, ansi);

    free(sprite->ansi[y][x]);
    sprite->ansi[y][x] = ansi0;

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    return 1;
}

int Sprite_appendAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi) {
    if (sprite == 0) return 0;
    if (ansi == 0) return 0;
    if (sprite->ansi == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);

    int ansiSize = strlen(ansi) + 1;
    if (sprite->ansi[y][x] == 0) {
        char* ansi0 = malloc(ansiSize);
        if (ansi0 == 0) return 0;

        strncpy(ansi0, ansi, ansiSize);

        sprite->ansi[y][x] = ansi0;
    }
    else {
        int oldLen = strlen(sprite->ansi[y][x]);
        int size = oldLen + ansiSize;
        char* ansi0 = realloc(sprite->ansi[y][x], size);
        if (ansi0 == 0) return 0;

        sprite->ansi[y][x] = ansi0;
        strncat(
            sprite->ansi[y][x], ansi,
            ansiSize - 1
        );                                   // Use strncat for safety
        sprite->ansi[y][x][size - 1] = '\0'; // Ensure null termination
    }

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    return 1;
}

int Sprite_fillAnsi(
    CmdFX_Sprite* sprite, int x, int y, char* ansi, int width, int height
) {
    if (sprite == 0) return 0;
    if (sprite->ansi == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;

    CmdFX_tryLockMutex(_SPRITE_DATA_MUTEX);

    for (int i = y; i < y + height; i++) {
        if (i >= sprite->height) break;
        for (int j = x; j < x + width; j++) {
            if (j >= sprite->width) break;
            char* ansi0 = malloc(strlen(ansi) + 1);
            snprintf(ansi0, strlen(ansi) + 1, "%s", ansi);

            free(sprite->ansi[i][j]);
            sprite->ansi[i][j] = ansi0;
        }
    }

    CmdFX_tryUnlockMutex(_SPRITE_DATA_MUTEX);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 1;
}

int Sprite_setAnsiAll(CmdFX_Sprite* sprite, char* ansi) {
    if (sprite == 0) return 0;
    if (sprite->ansi == 0) return 0;

    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            int n = strlen(ansi) + 1;
            char* ansi0 = malloc(n);
            strncpy(ansi0, ansi, n);

            free(sprite->ansi[i][j]);
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

CmdFX_Sprite* Sprite_createFilled(
    int width, int height, char c, char* ansi, int z
) {
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
    if (ansi != 0) Sprite_setAnsiAll(sprite, ansi);

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

            int n = sizeof(char) * (strlen(line) + 1);
            data[0] = malloc(n);
            if (data[0] == 0) {
                free(data);
                break;
            }

            strncpy(data[0], line, n);
            data[1] = 0;
            width = strlen(line);
            height = 1;
        }
        else {
            char** newData = realloc(data, sizeof(char*) * (height + 2));
            if (newData == 0) break;

            data = newData;
            int n = sizeof(char) * (strlen(line) + 1);
            data[height] = malloc(n);
            if (data[height] == 0) break;
            strncpy(data[height], line, n);

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
                free(sprite->ansi[i][j]);
            }
            free(sprite->ansi[i]);
        }
    }

    free(sprite->data);
    free(sprite->ansi);

    sprite->data = newData;
    sprite->ansi = newAnsi;
    sprite->width = width;
    sprite->height = height;

    return 1;
}

int Sprite_resize(CmdFX_Sprite* sprite, int width, int height) {
    return Sprite_resizeWithPadding(sprite, width, height, ' ');
}

int Sprite_resizeWithPadding(
    CmdFX_Sprite* sprite, int width, int height, char padding
) {
    if (sprite == 0) return 0;
    if (width < 1 || height < 1) return 0;

    // Remove Sprite if Drawn
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
    }

    if (!Sprite_resize0(sprite, width, height, padding)) {
        Sprite_draw0(sprite); // Redraw, resize failed
        return 0;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) {
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

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
            sprite->data[offsetY + i][offsetX + j] =
                sprite->data[top + i][left + j];
            if (sprite->ansi != 0) {
                sprite->ansi[offsetY + i][offsetX + j] =
                    sprite->ansi[top + i][left + j];
            }
        }
    }

    // Fill the remaining area with whitespace and reset ansi
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            if (i < offsetY || i >= offsetY + newHeight || j < offsetX ||
                j >= offsetX + newWidth) {
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
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
    }

    if (!Sprite_center0(sprite)) {
        Sprite_draw0(sprite); // Redraw, center failed
        return 0;
    }

    if (!Sprite_center0(sprite)) {
        Sprite_draw0(sprite); // Redraw, center failed
        return 0;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) {
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    return 1;
}

int Sprite_resizeAndCenter(CmdFX_Sprite* sprite, int width, int height) {
    if (sprite == 0) return 0;
    if (width < 1 || height < 1) return 0;

    // Remove Sprite if Drawn
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
    }

    if (!Sprite_resize0(sprite, width, height, ' ')) {
        Sprite_draw0(sprite); // Redraw, resize failed
        return 0;
    }

    if (!Sprite_center0(sprite)) {
        Sprite_draw0(sprite); // Redraw, center failed
        return 0;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) {
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    return 1;
}

// Utility Methods - Movement

void Sprite_moveTo(CmdFX_Sprite* sprite, int x, int y) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;
    if (x < 1 || y < 1) return;
    if (sprite->x == x && sprite->y == y) return;

    int width = Canvas_getWidth();
    int height = Canvas_getHeight();
    if (width > 0 && height > 0) {
        if (x + sprite->width > width || y + sprite->height > height) return;
    }

    CmdFX_tryLockMutex(_SPRITE_POSITION_MUTEX);
    CmdFX_tryLockMutex(_CANVAS_MUTEX);

    Sprite_remove0(sprite);
    sprite->x = x;
    sprite->y = y;
    Sprite_draw0(sprite);

    CmdFX_tryUnlockMutex(_SPRITE_POSITION_MUTEX);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Sprite_moveBy(CmdFX_Sprite* sprite, int dx, int dy) {
    if (sprite == 0) return;
    if (sprite->id == 0) return;
    if (dx == 0 && dy == 0) return;

    Sprite_moveTo(sprite, sprite->x + dx, sprite->y + dy);
}

// Utility Methods - Collisions

CmdFX_Sprite** Sprite_getCollidingSprites(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (_spriteCount < 2) return 0;

    int collidingCount = 0;
    int allocated = 4;

    CmdFX_Sprite** colliding = calloc(allocated + 1, sizeof(CmdFX_Sprite*));
    if (!colliding) return 0;

    for (int i = 0; i < _spriteCount; i++) {
        CmdFX_Sprite* other = _sprites[i];
        if (other->id == 0) continue;
        if (other->id == sprite->id) continue;

        if (Sprite_isColliding(sprite, other)) {
            if (collidingCount >= allocated) {
                allocated *= 2;
                CmdFX_Sprite** temp =
                    realloc(colliding, sizeof(CmdFX_Sprite*) * (allocated + 1));
                if (!temp) {
                    free(colliding);
                    return 0;
                }
                colliding = temp;

                // realloc a calloc
                for (int j = collidingCount; j < allocated + 1; j++)
                    colliding[j] = 0;
            }

            colliding[collidingCount++] = other;
        }
    }

    colliding[collidingCount] = 0;
    return colliding;
}

int Sprite_isColliding(CmdFX_Sprite* sprite1, CmdFX_Sprite* sprite2) {
    if (sprite1 == sprite2) return 0;
    if (sprite1 == 0) return 0;
    if (sprite2 == 0) return 0;

    if (sprite1->id == 0 || sprite2->id == 0) return 0;
    if (sprite1->id == sprite2->id) return 0;

    return sprite1->x <= sprite2->x + sprite2->width &&
           sprite1->x + sprite1->width >= sprite2->x &&
           sprite1->y <= sprite2->y + sprite2->height &&
           sprite1->y + sprite1->height >= sprite2->y;
}

int Sprite_isOnTop(CmdFX_Sprite* sprite, int x, int y) {
    if (sprite == 0) return 0;
    if (_spriteCount < 2) return 1;
    if (x < sprite->x || y < sprite->y) return 0;
    if (x >= sprite->x + sprite->width || y >= sprite->y + sprite->height)
        return 0;

    CmdFX_Sprite** collisions = Sprite_getCollidingSprites(sprite);
    if (collisions == 0) return 1;

    for (int i = 0; collisions[i] != 0; i++) {
        CmdFX_Sprite* other = collisions[i];
        if (x < other->x || y < other->y) continue;
        if (x >= other->x + other->width || y >= other->y + other->height)
            continue;

        if (other->z > sprite->z) {
            free(collisions);
            return 0;
        }
    }
    free(collisions);

    return 1;
}

int Sprite_isOnBottom(CmdFX_Sprite* sprite, int x, int y) {
    if (sprite == 0) return 0;
    if (sprite->id == 0) return 0;
    if (x < sprite->x || y < sprite->y) return 0;
    if (x >= sprite->x + sprite->width || y >= sprite->y + sprite->height)
        return 0;

    CmdFX_Sprite** collisions = Sprite_getCollidingSprites(sprite);
    if (collisions == 0) return 1;

    for (int i = 0; collisions[i] != 0; i++) {
        CmdFX_Sprite* other = collisions[i];
        if (x < other->x || y < other->y) continue;
        if (x >= other->x + other->width || y >= other->y + other->height)
            continue;

        if (other->z < sprite->z) {
            free(collisions);
            return 0;
        }
    }
    free(collisions);

    return 1;
}

// Utility Methods - Color

int Sprite_setForeground(CmdFX_Sprite* sprite, int x, int y, int rgb) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    snprintf(
        ansi, 22, "\033[38;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF,
        rgb & 0xFF
    );

    return Sprite_setAnsi(sprite, x, y, ansi);
}

int Sprite_setForeground256(CmdFX_Sprite* sprite, int x, int y, int color) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    snprintf(ansi, 14, "\033[38;5;%dm", color);

    return Sprite_setAnsi(sprite, x, y, ansi);
}

int Sprite_setForegroundAll(CmdFX_Sprite* sprite, int rgb) {
    if (sprite == 0) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    snprintf(
        ansi, 22, "\033[38;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF,
        rgb & 0xFF
    );

    return Sprite_setAnsiAll(sprite, ansi);
}

int Sprite_setForegroundAll256(CmdFX_Sprite* sprite, int color) {
    if (sprite == 0) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    snprintf(ansi, 14, "\033[38;5;%dm", color);

    return Sprite_setAnsiAll(sprite, ansi);
}

int Sprite_setBackground(CmdFX_Sprite* sprite, int x, int y, int rgb) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    snprintf(
        ansi, 22, "\033[48;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF,
        rgb & 0xFF
    );

    int res = Sprite_setAnsi(sprite, x, y, ansi);
    free(ansi);

    return res;
}

int Sprite_setBackground256(CmdFX_Sprite* sprite, int x, int y, int color) {
    if (sprite == 0) return 0;
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    snprintf(ansi, 14, "\033[48;5;%dm", color);

    int res = Sprite_setAnsi(sprite, x, y, ansi);
    free(ansi);

    return res;
}

int Sprite_setBackgroundAll(CmdFX_Sprite* sprite, int rgb) {
    if (sprite == 0) return 0;
    if (rgb < 0 || rgb > 0xFFFFFF) return 0;

    char* ansi = malloc(22);
    snprintf(
        ansi, 22, "\033[48;2;%d;%d;%dm", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF,
        rgb & 0xFF
    );

    int res = Sprite_setAnsiAll(sprite, ansi);
    free(ansi);

    return res;
}

int Sprite_setBackgroundAll256(CmdFX_Sprite* sprite, int color) {
    if (sprite == 0) return 0;
    if (color < 0 || color > 255) return 0;

    char* ansi = malloc(14);
    if (ansi == 0) return 0;

    snprintf(ansi, 14, "\033[48;5;%dm", color);

    int res = Sprite_setAnsiAll(sprite, ansi);
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

int Sprite_setGradient0(
    CmdFX_Sprite* sprite, int prefix, int x, int y, int width, int height,
    enum CmdFX_GradientDirection direction, int numColors, va_list* args
) {
    int* colors = (int*) malloc(sizeof(int) * (numColors + 1));
    if (colors == 0) return 0;

    for (int i = 0; i < numColors; i++) {
        colors[i] = va_arg(*args, int);
    }

    char*** gradient = String2DBuilder_create(width, height);
    if (gradient == 0) {
        free(colors);
        return 0;
    }
    if (prefix == 38)
        String2DBuilder_multiGradientForegroundFull(
            gradient, numColors, colors, direction
        );
    else
        String2DBuilder_multiGradientBackgroundFull(
            gradient, numColors, colors, direction
        );

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
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    return 1;
}

int Sprite_setForegroundGradient(
    CmdFX_Sprite* sprite, int x, int y, int width, int height,
    enum CmdFX_GradientDirection direction, int numColors, ...
) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(
        sprite, 38, x, y, width, height, direction, numColors, &args
    );
    va_end(args);

    return res;
}

int Sprite_setForegroundGradientAll(
    CmdFX_Sprite* sprite, enum CmdFX_GradientDirection direction, int numColors,
    ...
) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(
        sprite, 38, 0, 0, sprite->width, sprite->height, direction, numColors,
        &args
    );
    va_end(args);

    return res;
}

int Sprite_setBackgroundGradient(
    CmdFX_Sprite* sprite, int x, int y, int width, int height,
    enum CmdFX_GradientDirection direction, int numColors, ...
) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(
        sprite, 48, x, y, width, height, direction, numColors, &args
    );
    va_end(args);

    return res;
}

int Sprite_setBackgroundGradientAll(
    CmdFX_Sprite* sprite, enum CmdFX_GradientDirection direction, int numColors,
    ...
) {
    va_list args;
    va_start(args, numColors);
    int res = Sprite_setGradient0(
        sprite, 48, 0, 0, sprite->width, sprite->height, direction, numColors,
        &args
    );
    va_end(args);

    return res;
}

// Utility Methods - Transformations

int Sprite_rotate(CmdFX_Sprite* sprite, double radians) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
    }

    if (sprite->data != 0) Char2DBuilder_rotate(sprite->data, radians);

    if (sprite->ansi != 0) String2DBuilder_rotate(sprite->ansi, radians);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) {
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    return 0;
}

double Sprite_getRotationAngle(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0.0;
    if (sprite->data == 0) return 0.0;

    return Char2DBuilder_getRotationAngle(sprite->data);
}

int Sprite_hFlip(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
    }

    if (sprite->data != 0) Char2DBuilder_hFlip(sprite->data);

    if (sprite->ansi != 0) String2DBuilder_hFlip(sprite->ansi);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) {
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    return 0;
}

int Sprite_vFlip(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
    }

    if (sprite->data != 0) Char2DBuilder_vFlip(sprite->data);

    if (sprite->ansi != 0) String2DBuilder_vFlip(sprite->ansi);

    // Redraw Sprite if Drawn
    if (sprite->id != 0) {
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    return 0;
}

int Sprite_scale(CmdFX_Sprite* sprite, double scale) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
    }

    if (sprite->data != 0) {
        char** newData = Char2DBuilder_scale(sprite->data, scale);
        sprite->data = newData;
    }

    if (sprite->ansi != 0) {
        char*** newAnsi = String2DBuilder_scale(sprite->ansi, scale);
        sprite->ansi = newAnsi;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) {
        Sprite_draw0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    return 0;
}

int Sprite_transpose(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;

    // Remove Sprite if Drawn
    if (sprite->id != 0) {
        CmdFX_tryLockMutex(_CANVAS_MUTEX);
        Sprite_remove0(sprite);
        CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
    }

    if (sprite->data != 0) {
        char** newData = Char2DBuilder_transpose(sprite->data);
        sprite->data = newData;
    }

    if (sprite->ansi != 0) {
        char*** newAnsi = String2DBuilder_transpose(sprite->ansi);
        sprite->ansi = newAnsi;
    }

    // Redraw Sprite if Drawn
    if (sprite->id != 0) Sprite_draw0(sprite);

    return 0;
}
