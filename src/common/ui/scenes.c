#include <stdio.h>
#include <stdlib.h>

#include "cmdfx/core/scenes.h"
#include "cmdfx/core/util.h"
#include "cmdfx/ui/scenes.h"

#define _BUTTON_SCENE_REGISTRY_MUTEX 10
static CmdFX_Button*** _sceneButtons = 0;
static int* _sceneButtonsCount = 0;
static int*** _sceneButtonCoordinates = 0;

CmdFX_Button** Scene_getButtons(int uid) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return 0;
    if (_sceneButtons == 0) return 0;
    if (Scene_getRegisteredScene(uid) == 0) return 0;

    return _sceneButtons[uid];
}

int Scene_getButtonsCount(int uid) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (_sceneButtonsCount == 0) return 0;
    if (Scene_getRegisteredScene(uid) == 0) return 0;

    return _sceneButtonsCount[uid];
}

int* Scene_getButtonCoordinates(int uid, CmdFX_Button* button) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return 0;
    if (button == 0) return 0;
    if (_sceneButtonCoordinates == 0) return 0;
    if (Scene_getRegisteredScene(uid) == 0) return 0;

    int count = _sceneButtonsCount[uid];
    for (int i = 0; i < count; i++) {
        if (_sceneButtons[uid][i] == button) {
            return _sceneButtonCoordinates[uid][i];
        }
    }

    return 0;
}

void _checkSceneButtons(int uid) {
    CmdFX_tryLockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);
    if (_sceneButtons == 0) {
        _sceneButtons = (CmdFX_Button***) calloc(
            MAX_REGISTERED_SCENES, sizeof(CmdFX_Button**)
        );
        _sceneButtonsCount = (int*) calloc(MAX_REGISTERED_SCENES, sizeof(int));
    }

    if (_sceneButtons[uid] == 0) {
        _sceneButtons[uid] = (CmdFX_Button**) calloc(1, sizeof(CmdFX_Button*));
        _sceneButtonsCount[uid] = 0;
    }

    if (_sceneButtonCoordinates == 0) {
        _sceneButtonCoordinates =
            (int***) calloc(MAX_REGISTERED_SCENES, sizeof(int**));
    }

    if (_sceneButtonCoordinates[uid] == 0) {
        _sceneButtonCoordinates[uid] =
            (int**) calloc(MAX_BUTTONS_PER_SCENE, sizeof(int*));

        for (int i = 0; i < 4; i++) {
            _sceneButtonCoordinates[uid][i] = (int*) calloc(2, sizeof(int));
        }
    }
    CmdFX_tryUnlockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);
}

int Scene_addButton(int uid, CmdFX_Button* button, int x, int y) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (button == 0) return -1;
    if (Scene_getRegisteredScene(uid) == 0) return -1;

    int count = _sceneButtonsCount[uid];
    if (count >= MAX_BUTTONS_PER_SCENE) return -1;

    _checkSceneButtons(uid);

    CmdFX_tryLockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);
    _sceneButtonCoordinates[uid][count][0] = x;
    _sceneButtonCoordinates[uid][count][1] = y;

    _sceneButtons[uid] = (CmdFX_Button**) realloc(
        _sceneButtons[uid], sizeof(CmdFX_Button*) * (count + 1)
    );
    _sceneButtons[uid][count] = button;
    _sceneButtonsCount[uid]++;
    CmdFX_tryUnlockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);

    return 0;
}

int Scene_removeButton(int uid, CmdFX_Button* button) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (button == 0) return -1;
    if (Scene_getRegisteredScene(uid) == 0) return -1;

    _checkSceneButtons(uid);

    int count = _sceneButtonsCount[uid];
    for (int i = 0; i < count; i++) {
        if (_sceneButtons[uid][i] == button) {
            CmdFX_tryLockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);
            _sceneButtons[uid][i] = 0;
            for (int j = i; j < count - 1; j++) {
                _sceneButtons[uid][j] = _sceneButtons[uid][j + 1];
            }
            _sceneButtonsCount[uid]--;
            _sceneButtons[uid] = (CmdFX_Button**) realloc(
                _sceneButtons[uid],
                sizeof(CmdFX_Button*) * _sceneButtonsCount[uid]
            );

            _sceneButtonCoordinates[uid][i][0] = -1;
            _sceneButtonCoordinates[uid][i][1] = -1;
            CmdFX_tryUnlockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);
            break;
        }
    }

    return 0;
}

int Scene_removeButtonAt(int uid, int x, int y) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (_sceneButtons == 0) return -1;
    if (Scene_getRegisteredScene(uid) == 0) return -1;

    _checkSceneButtons(uid);

    int count = _sceneButtonsCount[uid];
    for (int i = 0; i < count; i++) {
        CmdFX_Button* button = _sceneButtons[uid][i];
        if (button == 0) continue;
        int rx = _sceneButtonCoordinates[uid][i][0];
        int ry = _sceneButtonCoordinates[uid][i][1];
        if ((button->x == x && button->y == y) || (rx == x && ry == y)) {
            Scene_removeButton(uid, button);
            break;
        }
    }

    return 0;
}

int Scene_removeAllButtons(int uid) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (_sceneButtons == 0) return -1;
    if (Scene_getRegisteredScene(uid) == 0) return -1;

    _checkSceneButtons(uid);

    int count = _sceneButtonsCount[uid];
    for (int i = 0; i < count; i++) {
        CmdFX_Button* button = _sceneButtons[uid][i];
        if (button == 0) continue;
        Scene_removeButton(uid, button);
    }

    CmdFX_tryLockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);
    free(_sceneButtons[uid]);
    _sceneButtons[uid] = 0;

    for (int i = 0; i < MAX_BUTTONS_PER_SCENE; i++) {
        free(_sceneButtonCoordinates[uid][i]);
    }
    free(_sceneButtonCoordinates[uid]);
    _sceneButtonCoordinates[uid] = 0;
    _sceneButtonsCount[uid] = 0;
    CmdFX_tryUnlockMutex(_BUTTON_SCENE_REGISTRY_MUTEX);

    return 0;
}