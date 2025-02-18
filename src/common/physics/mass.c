#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/mass.h"

int Sprite_getDefaultMass(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->data == 0) return -1;

    int mass = 0;
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            if (sprite->data[i] == 0) break;

            char c = sprite->data[i][j];
            if (c != ' ')
                mass += Engine_getCharacterMass(c);
        }
    }

    return mass;
}

int* _masses = 0;
int _massesCount = 0;

int Sprite_getMass(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;

    int id = sprite->uid - 1;
    if (_masses == 0) {
        _masses = calloc(id + 1, sizeof(int));
        if (_masses == 0) return -1;
    }

    if (_massesCount < id) {
        int* temp = realloc(_masses, sizeof(int) * (id + 1));
        if (temp == 0) return -1;

        _masses = temp;
        for (int i = _massesCount; i < id; i++) {
            _masses[i] = 0;
        }

        _massesCount = id + 1;
    }

    if (_masses[id] == 0) 
        return Sprite_getDefaultMass(sprite);

    return _masses[id];
}

int Sprite_setMass(CmdFX_Sprite* sprite, int mass) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;
    if (mass == -1) return Sprite_resetMass(sprite);
    if (mass < 0) return -1;

    int id = sprite->uid - 1;
    if (_masses == 0) {
        _masses = calloc(id + 1, sizeof(int));
        if (_masses == 0) return -1;
    }

    if (_massesCount < id) {
        int* temp = realloc(_masses, sizeof(int) * (id + 1));
        if (temp == 0) return -1;

        _masses = temp;
        for (int i = _massesCount; i < id; i++) {
            _masses[i] = 0;
        }

        _massesCount = id + 1;
    }

    _masses[id] = mass;
    return 0;
}

int Sprite_resetMass(CmdFX_Sprite* sprite) {
    if (sprite == 0) return -1;
    if (sprite->uid == 0) return -1;

    int id = sprite->uid - 1;
    if (_masses == 0) {
        _masses = calloc(id + 1, sizeof(int));
        if (_masses == 0) return -1;
    }

    if (_massesCount < id) {
        int* temp = realloc(_masses, sizeof(int) * (id + 1));
        if (temp == 0) return -1;

        _masses = temp;
        for (int i = _massesCount; i < id; i++) {
            _masses[i] = 0;
        }

        _massesCount = id + 1;
    }
    _masses[id] = 0;

    int count = Canvas_getDrawnSpritesCount();
    if (_massesCount != count) {
        int* temp = realloc(_masses, sizeof(int) * count);
        if (temp == 0) return -1;

        _masses = temp;
        _massesCount = count;
    }

    return 0;
}