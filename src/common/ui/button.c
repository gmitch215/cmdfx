#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/util.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/ui/button.h"

#define _BUTTON_REGISTRY_MUTEX 8
static CmdFX_Button** _registeredButtons = 0;
static int _registeredButtonsCount = 0;

#define _BUTTON_POSITION_MUTEX 9

CmdFX_Button** Canvas_getRegisteredButtons() {
    return _registeredButtons;
}

int Canvas_getRegisteredButtonsCount() {
    return _registeredButtonsCount;
}

CmdFX_Button* Button_create(CmdFX_Sprite* sprite, CmdFX_ButtonCallback callback) {
    if (sprite == 0) return 0;
    if (callback == 0) return 0;
    
    CmdFX_Button* button = (CmdFX_Button*) malloc(sizeof(CmdFX_Button));
    if (!button) return 0;
    
    button->sprite = sprite;
    button->id = -1;
    button->x = -1;
    button->y = -1;
    if (!Sprite_isStatic(sprite))
        Sprite_setStatic(sprite, 1);

    CmdFX_ButtonCallback* ptr = malloc(sizeof(CmdFX_ButtonCallback));
    if (!ptr) {
        free(button);
        return 0;
    }

    *ptr = callback;
    button->callback = ptr;

    return button;
}

void Button_free(CmdFX_Button* button) {
    if (button == 0) return;
    
    if (button->sprite != 0)
        Sprite_free(button->sprite);

    if (button->callback != 0)
        free(button->callback);
    
    free(button);
}

int Button_draw(int x, int y, CmdFX_Button* button) {
    if (button == 0) return -1;
    if (button->sprite == 0) return -1;

    if (Sprite_draw(x, y, button->sprite) != 0) return -1;

    CmdFX_tryLockMutex(_BUTTON_POSITION_MUTEX);
    button->x = x;
    button->y = y;
    CmdFX_tryUnlockMutex(_BUTTON_POSITION_MUTEX);

    CmdFX_tryLockMutex(_BUTTON_REGISTRY_MUTEX);
    int spritesCount = Canvas_getDrawnSpritesCount();
    if (_registeredButtons == 0) {
        _registeredButtons = malloc(sizeof(CmdFX_Button*) * spritesCount);
        if (_registeredButtons == 0) {
            CmdFX_tryUnlockMutex(_BUTTON_REGISTRY_MUTEX);
            return -1;
        }
        _registeredButtonsCount = spritesCount;
    }
    if (_registeredButtonsCount != spritesCount) {
        CmdFX_Button** temp = realloc(_registeredButtons, sizeof(CmdFX_Button*) * spritesCount);
        if (temp == 0) {
            CmdFX_tryUnlockMutex(_BUTTON_REGISTRY_MUTEX);
            return -1;
        }
        _registeredButtons = temp;
        _registeredButtonsCount = spritesCount;
    }

    button->id = button->sprite->id - 1;
    _registeredButtons[button->id] = button;
    CmdFX_tryUnlockMutex(_BUTTON_REGISTRY_MUTEX); 

    return 0;
}

int Button_remove(CmdFX_Button* button) {
    if (button == 0) return -1;
    if (button->sprite == 0) return -1;

    CmdFX_tryLockMutex(_BUTTON_REGISTRY_MUTEX);
    _registeredButtons[button->id] = 0;
    button->id = -1;
    CmdFX_tryUnlockMutex(_BUTTON_REGISTRY_MUTEX);

    Sprite_remove(button->sprite);
    return 0;
}

int Button_isHidden(CmdFX_Button* button) {
    if (button == 0) return 1;
    if (button->sprite == 0) return 1;

    return button->sprite->id == 0;
}

int Button_hide(CmdFX_Button* button) {
    if (button == 0) return -1;
    if (button->sprite == 0) return -1;

    Sprite_remove(button->sprite);
    return 0;
}

int Button_show(CmdFX_Button* button) {
    if (button == 0) return -1;
    if (button->sprite == 0) return -1;

    return Sprite_draw(button->x, button->y, button->sprite);
}

CmdFX_Button** Canvas_getAllButtonsAt(int x, int y) {
    if (x < 0 || y < 0) return 0;
    if (_registeredButtons == 0) return 0;
    if (_registeredButtonsCount < 1) return 0;

    CmdFX_Button** matching = calloc(_registeredButtonsCount, sizeof(CmdFX_Sprite*));

    for (int i = 0; i < _registeredButtonsCount; i++) {
        CmdFX_Button* button = _registeredButtons[i];
        if (button->id == 0) continue;

        if (x >= button->x && x < button->x + button->sprite->width)
            if (y >= button->y && y < button->y + button->sprite->height)
                matching[i] = button;
    }

    return matching;
}

CmdFX_Button* Canvas_getButtonAt(int x, int y) {
    if (x < 0 || y < 0) return 0;
    if (_registeredButtons == 0) return 0;
    if (_registeredButtonsCount < 1) return 0;

    CmdFX_Button** matching = Canvas_getAllButtonsAt(x, y);

    CmdFX_Button* button = 0;
    for (int i = 0; i < _registeredButtonsCount; i++) {
        if (matching[i] == 0) continue;
        if (button == 0) {
            button = matching[i];
            continue;
        }

        if (matching[i]->sprite->z > button->sprite->z) button = matching[i];
    }

    free(matching);
    return 0;
}

int Button_setData(CmdFX_Button* button, char** data, char*** ansi) {
    if (button == 0) return -1;
    if (button->sprite == 0) return -1;
    if (data == 0) return -1;

    CmdFX_Sprite* sprite = button->sprite;
    for (int i = 0; i < sprite->height; i++) {
        if (sprite->data[i] == 0) continue;
        free(sprite->data[i]);
    }
    free(sprite->data);
    sprite->data = data;

    if (ansi != 0) {
        int height = getStringArrayHeight(sprite->ansi);
        int width = getStringArrayWidth(sprite->ansi);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) free(sprite->ansi[i][j]);
            free(sprite->ansi[i]);
        }
        free(sprite->ansi);
        
        sprite->ansi = ansi;
    }

    if (Sprite_draw(button->x, button->y, sprite) != 0) return -1;

    return 0;
}