#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/core/costumes.h"

CmdFX_SpriteCostumes** _costumes = 0;
int _costumeCount = 0;

CmdFX_SpriteCostumes* Sprite_createCostumes(CmdFX_Sprite* sprite, int costumeCount) {
    if (costumeCount < 1 || costumeCount > MAX_SPRITE_COSTUMES) return 0;
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;

    if (_costumes == 0) {
        _costumes = calloc(4, sizeof(CmdFX_SpriteCostumes));
        if (!_costumes) return 0;
        _costumeCount = 4;
    }

    if (_costumeCount < sprite->uid) {
        CmdFX_SpriteCostumes** temp = realloc(_costumes, _costumeCount * 2 * sizeof(CmdFX_SpriteCostumes));
        if (!temp) return 0;
        for (int i = _costumeCount; i < _costumeCount * 2; i++) temp[i] = 0; // crealloc
        _costumes = temp;
        _costumeCount *= 2;
    }

    CmdFX_SpriteCostumes* spriteCostumes = malloc(sizeof(CmdFX_SpriteCostumes));
    
    spriteCostumes->costumes = (char***) calloc(costumeCount, sizeof(char**));
    spriteCostumes->ansiCostumes = (char****) calloc(costumeCount, sizeof(char***));

    spriteCostumes->costumes[0] = createArrayCopy(sprite->data);
    if (sprite->ansi != 0) spriteCostumes->ansiCostumes[0] = createAnsiArrayCopy(sprite->ansi);

    _costumes[sprite->uid] = spriteCostumes;
    return spriteCostumes;
}

CmdFX_SpriteCostumes* Sprite_getCostumes(CmdFX_Sprite* sprite) {
    if (sprite == 0) return 0;
    if (sprite->data == 0) return 0;
    if (_costumes == 0 || _costumeCount == 0) return 0;
    if (sprite->uid > _costumeCount) return 0;

    return _costumes[sprite->uid];
}