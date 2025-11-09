#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/core/builder.h"
#include "cmdfx/core/canvas.h"
#include "cmdfx/core/costumes.h"
#include "cmdfx/core/sprites.h"

CmdFX_SpriteCostumes** _costumes = 0;
int _costumeCount = 0;

CmdFX_SpriteCostumes* Sprite_createCostumes(
    CmdFX_Sprite* sprite, int costumeCount
) {
    if (costumeCount < 1 || costumeCount > MAX_SPRITE_COSTUMES) return 0;
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;

    if (_costumes == 0) {
        _costumes = calloc(4, sizeof(CmdFX_SpriteCostumes));
        if (!_costumes) return 0;
        _costumeCount = 4;
    }

    int id = sprite->uid - 1;
    if (_costumeCount < id) {
        CmdFX_SpriteCostumes** temp = realloc(
            _costumes, _costumeCount * 2 * sizeof(CmdFX_SpriteCostumes)
        );
        if (!temp) return 0;
        for (int i = _costumeCount; i < _costumeCount * 2; i++)
            temp[i] = 0; // crealloc
        _costumes = temp;
        _costumeCount *= 2;
    }

    if (_costumes[id] != 0) return _costumes[id];

    CmdFX_SpriteCostumes* spriteCostumes = malloc(sizeof(CmdFX_SpriteCostumes));
    if (!spriteCostumes) return 0;

    spriteCostumes->costumeCount = costumeCount;
    spriteCostumes->costumes =
        (char***) calloc(costumeCount + 1, sizeof(char**));
    spriteCostumes->ansiCostumes =
        (char****) calloc(costumeCount + 1, sizeof(char***));

    if (!spriteCostumes->costumes || !spriteCostumes->ansiCostumes) {
        free(spriteCostumes->costumes);
        free(spriteCostumes->ansiCostumes);
        free(spriteCostumes);
        return 0;
    }

    spriteCostumes->costumes[0] = createCharArrayCopy(sprite->data);
    if (sprite->ansi != 0)
        spriteCostumes->ansiCostumes[0] = createStringArrayCopy(sprite->ansi);

    _costumes[id] = spriteCostumes;
    return spriteCostumes;
}

CmdFX_SpriteCostumes* Sprite_getCostumes(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (_costumes == 0 || _costumeCount == 0) return 0;
    if (sprite->uid > _costumeCount) return 0;

    return _costumes[sprite->uid - 1];
}

int Sprite_setCostumeAt(
    CmdFX_Sprite* sprite, int index, char** costume, char*** ansiCostume
) {
    if (sprite == 0) return -1;
    if (costume == 0) return -1;
    if (_costumeCount < sprite->uid) return -1;

    CmdFX_SpriteCostumes* spriteCostumes = _costumes[sprite->uid - 1];
    if (spriteCostumes == 0) return -1;
    if (index < 0 || spriteCostumes->costumeCount < index) return -2;

    char** oldData = spriteCostumes->costumes[index];
    if (oldData != 0) {
        int height = getCharArrayHeight(oldData);
        for (int i = 0; i < height; i++) free(oldData[i]);
        free(oldData);
    }
    spriteCostumes->costumes[index] = costume;

    if (ansiCostume != 0) {
        char*** oldAnsi = spriteCostumes->ansiCostumes[index];
        if (oldAnsi != 0) {
            int height = getStringArrayHeight(oldAnsi);
            int width = getStringArrayWidth(oldAnsi);

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) free(oldAnsi[i][j]);
                free(oldAnsi[i]);
            }
            free(oldAnsi);
        }
        spriteCostumes->ansiCostumes[index] = ansiCostume;
    }

    return 0;
}

char** Sprite_getCharCostume(CmdFX_Sprite* sprite, int costumeIndex) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (_costumes == 0 || _costumeCount == 0) return 0;
    if (sprite->uid > _costumeCount) return 0;

    CmdFX_SpriteCostumes* costumes = _costumes[sprite->uid - 1];
    if (costumes == 0) return 0;

    if (costumeIndex < 0 || costumeIndex >= MAX_SPRITE_COSTUMES) return 0;
    if (costumes->costumes == 0) return 0;

    return costumes->costumes[costumeIndex];
}

char*** Sprite_getAnsiCostume(CmdFX_Sprite* sprite, int costumeIndex) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (_costumes == 0 || _costumeCount == 0) return 0;
    if (sprite->uid > _costumeCount) return 0;

    CmdFX_SpriteCostumes* costumes = _costumes[sprite->uid - 1];
    if (costumes == 0) return 0;

    if (costumeIndex < 0 || costumeIndex >= MAX_SPRITE_COSTUMES) return 0;
    if (costumes->ansiCostumes == 0) return 0;

    return costumes->ansiCostumes[costumeIndex];
}

int Sprite_switchCostumeTo(CmdFX_Sprite* sprite, int costumeIndex) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;
    if (_costumes == 0 || _costumeCount == 0) return -1;
    if (sprite->uid > _costumeCount) return -1;

    CmdFX_SpriteCostumes* spriteCostumes = _costumes[sprite->uid - 1];
    if (spriteCostumes == 0) return -1;

    if (costumeIndex < 0 || costumeIndex >= spriteCostumes->costumeCount)
        return -1;
    if (spriteCostumes->costumes == 0) return -1;
    if (spriteCostumes->costumes[costumeIndex] == 0) return -1;

    sprite->data = spriteCostumes->costumes[costumeIndex];
    sprite->ansi = spriteCostumes->ansiCostumes[costumeIndex];

    return 0;
}

int Sprite_addCostume(
    CmdFX_Sprite* sprite, char** costume, char*** ansiCostume
) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;
    if (_costumes == 0 || _costumeCount == 0) return -1;
    if (sprite->uid > _costumeCount) return -1;

    CmdFX_SpriteCostumes* spriteCostumes = _costumes[sprite->uid - 1];
    if (spriteCostumes == 0) return -1;

    if (spriteCostumes->costumes == 0) return -1;
    if (spriteCostumes->ansiCostumes == 0) return -1;

    for (int i = 0; i < spriteCostumes->costumeCount; i++)
        if (spriteCostumes->costumes[i] == 0) {
            if (i == spriteCostumes->costumeCount - 1) {
                char*** newCostumes =
                    realloc(spriteCostumes->costumes, (i + 2) * sizeof(char**));
                if (newCostumes == 0) return -1;

                char**** newAnsiCostumes = realloc(
                    spriteCostumes->ansiCostumes, (i + 2) * sizeof(char***)
                );
                if (newAnsiCostumes == 0) return -1;

                spriteCostumes->costumes = newCostumes;
                spriteCostumes->ansiCostumes = newAnsiCostumes;

                spriteCostumes->costumes[i + 1] = 0;
                spriteCostumes->ansiCostumes[i + 1] = 0;
            }

            spriteCostumes->costumes[i] = createCharArrayCopy(costume);
            spriteCostumes->ansiCostumes[i] =
                createStringArrayCopy(ansiCostume);
            spriteCostumes->costumeCount++;
            return 0;
        }

    return -1;
}

int Sprite_removeCostume(CmdFX_Sprite* sprite, int costumeIndex) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;
    if (_costumes == 0 || _costumeCount == 0) return -1;
    if (sprite->uid > _costumeCount) return -1;

    CmdFX_SpriteCostumes* spriteCostumes = _costumes[sprite->uid - 1];
    if (spriteCostumes == 0) return -1;

    if (spriteCostumes->costumes == 0) return -1;
    if (spriteCostumes->ansiCostumes == 0) return -1;

    if (costumeIndex < 0 || costumeIndex >= spriteCostumes->costumeCount)
        return -1;

    if (spriteCostumes->costumes[costumeIndex] != 0) {
        for (int j = 0;
             j < getCharArrayHeight(spriteCostumes->costumes[costumeIndex]);
             j++)
            free(spriteCostumes->costumes[costumeIndex][j]);

        free(spriteCostumes->costumes[costumeIndex]);
    }
    free(spriteCostumes->costumes[costumeIndex]);

    if (spriteCostumes->ansiCostumes[costumeIndex] != 0) {
        for (int j = 0;
             j <
             getStringArrayHeight(spriteCostumes->ansiCostumes[costumeIndex]);
             j++) {
            for (int i = 0; i < getStringArrayWidth(
                                    spriteCostumes->ansiCostumes[costumeIndex]
                                );
                 i++)
                free(spriteCostumes->ansiCostumes[costumeIndex][j][i]);

            free(spriteCostumes->ansiCostumes[costumeIndex][j]);
        }

        free(spriteCostumes->ansiCostumes[costumeIndex]);
    }
    free(spriteCostumes->ansiCostumes[costumeIndex]);

    for (int i = costumeIndex; i < spriteCostumes->costumeCount - 1; i++) {
        spriteCostumes->costumes[i] = spriteCostumes->costumes[i + 1];
        spriteCostumes->ansiCostumes[i] = spriteCostumes->ansiCostumes[i + 1];
    }

    spriteCostumes->costumes[spriteCostumes->costumeCount - 1] = 0;
    spriteCostumes->ansiCostumes[spriteCostumes->costumeCount - 1] = 0;
    spriteCostumes->costumeCount--;

    return 0;
}

int Sprite_resetCostumes(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;
    if (_costumes == 0 || _costumeCount == 0) return -1;
    if (sprite->uid > _costumeCount) return -1;

    CmdFX_SpriteCostumes* spriteCostumes = _costumes[sprite->uid - 1];
    if (spriteCostumes == 0) return -1;

    if (spriteCostumes->costumes == 0) return -1;
    if (spriteCostumes->ansiCostumes == 0) return -1;

    if (spriteCostumes->costumes[0] == 0) return -1;
    if (spriteCostumes->ansiCostumes[0] == 0) return -1;

    for (int i = 1; i < spriteCostumes->costumeCount; i++) {
        if (spriteCostumes->costumes[i] != 0) {
            for (int j = 0; j < getCharArrayHeight(spriteCostumes->costumes[i]);
                 j++)
                free(spriteCostumes->costumes[i][j]);

            free(spriteCostumes->costumes[i]);
        }
        free(spriteCostumes->costumes[i]);

        if (spriteCostumes->ansiCostumes[i] != 0) {
            for (int j = 0;
                 j < getStringArrayHeight(spriteCostumes->ansiCostumes[i]);
                 j++) {
                for (int k = 0;
                     k < getStringArrayWidth(spriteCostumes->ansiCostumes[i]);
                     k++)
                    free(spriteCostumes->ansiCostumes[i][j][k]);

                free(spriteCostumes->ansiCostumes[i][j]);
            }
            free(spriteCostumes->ansiCostumes[i]);
        }
        free(spriteCostumes->ansiCostumes[i]);
    }

    spriteCostumes->costumeCount = 1;
    return 0;
}

int Sprite_freeCostumes(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;
    if (_costumes == 0 || _costumeCount == 0) return -1;
    if (sprite->uid > _costumeCount) return -1;

    CmdFX_SpriteCostumes* spriteCostumes = _costumes[sprite->uid - 1];
    if (spriteCostumes == 0) return -1;

    if (spriteCostumes->costumes == 0) return -1;
    if (spriteCostumes->ansiCostumes == 0) return -1;

    for (int i = 0; i < spriteCostumes->costumeCount; i++) {
        char** data = spriteCostumes->costumes[i];
        if (data != 0) {
            int height = getCharArrayHeight(data);
            for (int j = 0; j < height; j++) free(data[j]);
            free(data);
        }

        char*** ansi = spriteCostumes->ansiCostumes[i];
        if (ansi != 0) {
            int height = getStringArrayHeight(ansi);
            int width = getStringArrayWidth(ansi);
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < width; k++) free(ansi[j][k]);
                free(ansi[j]);
            }
            free(ansi);
        }
    }

    free(spriteCostumes->costumes);
    free(spriteCostumes->ansiCostumes);
    free(spriteCostumes);

    _costumes[sprite->uid - 1] = 0;
    return 0;
}

int Sprite_getCurrentCostumeIndex(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;
    if (_costumes == 0 || _costumeCount == 0) return -1;
    if (sprite->uid > _costumeCount) return -1;

    CmdFX_SpriteCostumes* spriteCostumes = _costumes[sprite->uid - 1];
    if (spriteCostumes == 0) return -1;

    if (spriteCostumes->costumes == 0) return -1;
    if (spriteCostumes->ansiCostumes == 0) return -1;

    for (int i = 0; i < spriteCostumes->costumeCount; i++) {
        char** data = spriteCostumes->costumes[i];
        if (compareCharArrays(data, sprite->data) != 0) continue;

        char*** ansi = spriteCostumes->ansiCostumes[i];
        if (compareStringArrays(ansi, sprite->ansi) != 0) continue;

        return i;
    }

    return -1;
}

int Sprite_resetAllCostumes() {
    if (_costumes == 0) return -1;
    if (_costumeCount == 0) return -1;

    CmdFX_Sprite** sprites = Canvas_getDrawnSprites();
    for (int i = 0; i < Canvas_getDrawnSpritesCount(); i++) {
        CmdFX_Sprite* sprite = sprites[i];
        Sprite_resetCostumes(sprite);

        free(_costumes[sprite->uid - 1]);
        _costumes[sprite->uid - 1] = 0;
    }

    free(_costumes);
    _costumes = 0;
    _costumeCount = 0;

    return 0;
}
