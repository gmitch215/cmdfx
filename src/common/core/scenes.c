#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/scenes.h"
#include "cmdfx/core/util.h"
#include "cmdfx/ui/button.h"
#include "cmdfx/ui/scenes.h"

#define _CANVAS_MUTEX 7
CmdFX_Scene** _drawnScenes = 0;
int _drawnScenesCount = 0;
int** _drawnSceneBounds = 0;

CmdFX_Scene** Scene_getDrawnScenes() {
    return _drawnScenes;
}

int Scene_getDrawnScenesCount() {
    return _drawnScenesCount;
}

CmdFX_Scene* Scene_create(int width, int height) {
    if (width < 1 || height < 1) return 0;

    CmdFX_Scene* scene = (CmdFX_Scene*) malloc(sizeof(CmdFX_Scene));
    if (scene == 0) return 0;

    char** data = Char2DBuilder_create(width, height);
    if (data == 0) {
        free(scene);
        return 0;
    }

    char*** ansiData = String2DBuilder_create(width, height);
    if (ansiData == 0) {
        free(scene);
        
        for (int i = 0; i < height; i++) free(data[i]);
        free(data);

        return 0;
    }

    scene->uid = -1;
    scene->width = width;
    scene->height = height;
    scene->data = data;
    scene->ansiData = ansiData;
    
    scene->x = -1;
    scene->y = -1;
    scene->z = 0;

    return scene;
}

CmdFX_Scene* Scene_createFilled(int width, int height, char c, char* ansi, int z) {
    if (width < 1 || height < 1) return 0;

    CmdFX_Scene* scene = Scene_create(width, height);
    if (scene == 0) return 0;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            scene->data[i][j] = c;
            if (ansi != 0) scene->ansiData[i][j] = ansi;
        }

    scene->z = z;

    return scene;
}

CmdFX_Scene* Scene_createFromData(char** data, char*** ansiData) {
    if (data == 0) return 0;

    int height = getCharArrayHeight(data);
    int width = getCharArrayWidth(data);

    if (height < 1 || width < 1) return 0;

    CmdFX_Scene* scene = (CmdFX_Scene*) malloc(sizeof(CmdFX_Scene));
    if (scene == 0) return 0;

    scene->uid = -1;
    scene->width = width;
    scene->height = height;
    scene->data = data;

    if (ansiData != 0) {
        int ansiHeight = getStringArrayHeight(ansiData);
        int ansiWidth = getStringArrayWidth(ansiData);
    
        if (height != ansiHeight || width != ansiWidth) return 0;
    }
    scene->ansiData = ansiData;

    scene->x = -1;
    scene->y = -1;
    scene->z = 0;

    return scene;
}

int Scene_clear(CmdFX_Scene* scene) {
    if (scene == 0) return -1;

    int width = scene->width;
    int height = scene->height;

    int ansi = scene->ansiData != 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            scene->data[i][j] = ' ';
            if (ansi) scene->ansiData[i][j] = 0;
        }
    
    return 0;
}

void Scene_draw0(CmdFX_Scene* scene, int x, int y, int x1, int y1, int x2, int y2) {
    int _x1 = clamp_i(x1, 0, scene->width);
    int _y1 = clamp_i(y1, 0, scene->height);
    int _x2 = clamp_i(x2, 0, scene->width);
    int _y2 = clamp_i(y2, 0, scene->height);

    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    
    for (int i = _y1; i < _y2; i++)
        for (int j = _x1; j < _x2; j++) {
            char c = scene->data[i][j];
            if (c == 0) break;
            if (c == ' ') continue;

            // original values ensure that the scene is drawn in the correct position
            int cx = (x + j) - x1;
            int cy = (y + i) - y1;

            if (!Scene_isOnTopAt(scene, cx, cy)) continue;

            Canvas_setCursor(cx, cy);
            putchar(c);

            if (scene->ansiData != 0) {
                char* ansi = scene->ansiData[i][j];
                if (ansi != 0) Canvas_setAnsiCurrent(ansi);
            }
        }
    
    // draw buttons on the scene
    CmdFX_Button** buttons = Scene_getButtons(scene->uid);
    int buttonCount = Scene_getButtonsCount(scene->uid);
    for (int i = 0; i < buttonCount; i++) {
        CmdFX_Button* button = buttons[i];
        if (button == 0) continue;

        int* coords = Scene_getButtonCoordinates(scene->uid, button);
        if (coords == 0) continue;

        // original values ensure that the scene is drawn in the correct position
        int bx = (coords[0] + x) - x1;
        int by = (coords[1] + y) - y1;

        if (bx < 0 || by < 0) continue;
        if (bx >= scene->width || by >= scene->height) continue;

        Canvas_setCursor(bx, by);
        Button_draw(bx, by, button);
    }
    
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

void Scene_draw1(CmdFX_Scene* scene, int x, int y, int x1, int y1, int x2, int y2) {
    if (_drawnScenes == 0) {
        _drawnScenes = (CmdFX_Scene**) malloc(sizeof(CmdFX_Scene*));
    } else {
        _drawnScenes = (CmdFX_Scene**) realloc(_drawnScenes, sizeof(CmdFX_Scene*) * (_drawnScenesCount + 1));
    }

    _drawnScenes[_drawnScenesCount] = scene;
    _drawnScenesCount++;

    if (scene->uid > -1) {
        if (_drawnSceneBounds == 0) {
            _drawnSceneBounds = (int**) calloc(MAX_REGISTERED_SCENES, sizeof(int*));
        }

        int* bounds = (int*) calloc(4, sizeof(int));
        bounds[0] = x1;
        bounds[1] = y1;
        bounds[2] = x2;
        bounds[3] = y2;
        _drawnSceneBounds[scene->uid] = bounds;
    }
}

int Scene_draw(CmdFX_Scene* scene, int x, int y) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (scene->x != -1 && scene->y != -1) Scene_remove(scene);

    scene->x = x;
    scene->y = y;
    Scene_draw0(scene, x, y, 0, 0, scene->width, scene->height);
    Scene_draw1(scene, x, y, 0, 0, scene->width, scene->height);
    return 0;
}

int Scene_drawPortion(CmdFX_Scene* scene, int x, int y, int sx, int sy, int width, int height) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (scene->x != -1 && scene->y != -1) Scene_remove(scene);

    scene->x = x;
    scene->y = y;
    Scene_draw0(scene, x, y, sx, sy, width, height);
    Scene_draw1(scene, x, y, sx, sy, width, height);
    return 0;
}

CmdFX_Scene* Scene_getSceneAt(int x, int y) {
    if (x < 0 || y < 0) return 0;
    if (_drawnScenes == 0) return 0;
    if (_drawnScenesCount == 0) return 0;

    int width = Canvas_getWidth();
    int height = Canvas_getHeight();

    if (width > 0 && height > 0) {
        if (x >= width || y >= height) return 0;
    }

    CmdFX_Scene** stack = malloc(sizeof(CmdFX_Scene*) * _drawnScenesCount);
    if (stack == 0) return 0;

    int stackCount = 0;
    for (int i = 0; i < _drawnScenesCount; i++) {
        CmdFX_Scene* scene = _drawnScenes[i];
        if (scene == 0) continue;
        if (scene->x == -1 && scene->y == -1) continue;

        if (x >= scene->x && x < scene->x + scene->width && y >= scene->y && y < scene->y + scene->height) {
            stack[stackCount] = scene;
            stackCount++;
        }
    }

    if (stackCount == 0) {
        free(stack);
        return 0;
    }
    CmdFX_Scene* top = stack[0];
    for (int i = 1; i < stackCount; i++) {
        CmdFX_Scene* scene = stack[i];
        if (scene->z > top->z) top = scene;
    }
    free(stack);

    return top;
}

int Scene_isOnTop(CmdFX_Scene* scene) {
    if (scene == 0) return -1;
    if (scene->x == -1 && scene->y == -1) return 0;

    CmdFX_Scene* top = Scene_getSceneAt(scene->x, scene->y);
    if (top == 0) return 0;

    return top == scene ? 1 : 0;
}

int Scene_isOnTopAt(CmdFX_Scene* scene, int x, int y) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;

    CmdFX_Scene* top = Scene_getSceneAt(x, y);
    if (top == 0) return 0;

    return top == scene ? 1 : 0;
}

int Scene_isOnBottom(CmdFX_Scene* scene) {
    if (scene == 0) return -1;
    if (scene->x == -1 && scene->y == -1) return 0;

    return Scene_isOnBottomAt(scene, scene->x, scene->y);
}

int Scene_isOnBottomAt(CmdFX_Scene* scene, int x, int y) {
    if (scene == 0) return 0;
    if (x < 0 || y < 0) return 0;

    if (_drawnScenesCount == 0) return 0;
    if (_drawnScenes == 0) return 0;
    if (scene->x == -1 && scene->y == -1) return 0;

    CmdFX_Scene** bottomScenes = calloc(_drawnScenesCount, sizeof(CmdFX_Scene*));
    if (bottomScenes == 0) return 0;

    int c = 0;
    for (int i = 0; i < _drawnScenesCount; i++) {
        CmdFX_Scene* scene = _drawnScenes[i];
        if (scene == 0) continue;
        if (scene->x == -1 && scene->y == -1) continue;

        if (x >= scene->x && x < scene->x + scene->width && y >= scene->y && y < scene->y + scene->height) {
            bottomScenes[c++] = scene;
        }
    }

    CmdFX_Scene* bottom = bottomScenes[0];
    if (bottom == 0 || c == 0) {
        free(bottomScenes);
        return 0;
    }

    for (int i = 1; i < c; i++) {
        CmdFX_Scene* scene = bottomScenes[i];
        if (scene->z < bottom->z) bottom = scene;
    }

    printf("count: %d\n", c);

    free(bottomScenes);
    return bottom == scene ? 1 : 0;
}

void Scene_remove0(CmdFX_Scene* scene) {
    CmdFX_tryLockMutex(_CANVAS_MUTEX);
    int width = scene->width;
    int height = scene->height;
    if (scene->uid > -1 && _drawnSceneBounds != 0) {
        int* bounds = _drawnSceneBounds[scene->uid];
        if (bounds != 0) {
            width = bounds[2] - bounds[0];
            height = bounds[3] - bounds[1];
        }
    }

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            Canvas_setCursor(scene->x + j, scene->y + i);
            putchar(' ');
            printf("\033[0m");
        }
    
    // remove buttons on the scene
    CmdFX_Button** buttons = Scene_getButtons(scene->uid);
    int buttonCount = Scene_getButtonsCount(scene->uid);
    for (int i = 0; i < buttonCount; i++) {
        CmdFX_Button* button = buttons[i];
        if (button == 0) continue;
        if (button->id == -1) continue; // button already removed
        Button_remove(button);
    }
    
    fflush(stdout);
    CmdFX_tryUnlockMutex(_CANVAS_MUTEX);
}

int Scene_remove(CmdFX_Scene* scene) {
    if (scene == 0) return -1;
    if (scene->x == -1 && scene->y == -1) return 0; // already removed, default to success

    Scene_remove0(scene);
    scene->x = -1;
    scene->y = -1;

    for (int i = 0; i < _drawnScenesCount; i++)
        if (_drawnScenes[i] == scene) {
            for (int j = i; j < _drawnScenesCount - 1; j++)
                _drawnScenes[j] = _drawnScenes[j + 1];

            if (scene->uid > -1 && _drawnSceneBounds != 0) {
                free(_drawnSceneBounds[scene->uid]);
                _drawnSceneBounds[scene->uid] = 0;
            }

            _drawnScenes[_drawnScenesCount - 1] = 0;
            _drawnScenesCount--;
            break;
        }

    return 0;
}

int Scene_isEmpty(CmdFX_Scene* scene) {
    if (scene == 0) return -1;

    int width = scene->width;
    int height = scene->height;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (scene->data[i][j] != ' ') return 0;
    
    return 1;
}

int Scene_switchTo(CmdFX_Scene* scene, int x, int y) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;

    for (int i = 0; i < _drawnScenesCount; i++)
        Scene_remove(_drawnScenes[i]);

    Scene_draw(scene, x, y);
    return 0;
}

int Scene_free(CmdFX_Scene* scene) {
    if (scene == 0) return 0; // free(0) is a no-op

    Scene_remove(scene);

    int width = scene->width;
    int height = scene->height;

    for (int i = 0; i < height; i++) free(scene->data[i]);
    free(scene->data);

    if (scene->ansiData != 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++)
                free(scene->ansiData[i][j]);
            free(scene->ansiData[i]);
        }
    }
    free(scene->ansiData);

    if (scene->uid > -1 && _drawnSceneBounds != 0) {
        free(_drawnSceneBounds[scene->uid]);
        _drawnSceneBounds[scene->uid] = 0;
    }

    // remove buttons on the scene
    Scene_removeAllButtons(scene->uid);

    // unregister scene
    Scene_unregister(scene);

    free(scene);
    return 0;
}

CmdFX_Scene** _registeredScenes = 0;

CmdFX_Scene** Scene_getRegisteredScenes() {
    return _registeredScenes;
}

int Scene_getRegisteredScenesCount() {
    int n = 0;
    for (int i = 0; i < MAX_REGISTERED_SCENES; i++)
        if (_registeredScenes[i] != 0) n++;
    
    return n;
}

CmdFX_Scene* Scene_getRegisteredScene(int uid) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return 0;
    if (_registeredScenes == 0) return 0;
    
    for (int i = 0; i < MAX_REGISTERED_SCENES; i++)
        if (_registeredScenes[i] != 0 && _registeredScenes[i]->uid == uid) return _registeredScenes[i];
    
    return 0;
}

int Scene_register(CmdFX_Scene* scene) {
    if (scene == 0) return -1;
    if (scene->uid != -1) return scene->uid;

    if (_registeredScenes == 0) {
        _registeredScenes = (CmdFX_Scene**) calloc(MAX_REGISTERED_SCENES, sizeof(CmdFX_Scene*));
    }

    int cuid = 0;
    while (_registeredScenes[cuid] != 0) cuid++;
    if (cuid >= MAX_REGISTERED_SCENES) return -1;

    scene->uid = cuid;
    _registeredScenes[cuid] = scene;

    return scene->uid;
}

int Scene_unregister(CmdFX_Scene* scene) {
    if (scene == 0) return -1;
    if (scene->uid == -1) return 0;

    int old = scene->uid;
    _registeredScenes[scene->uid] = 0;
    scene->uid = -1;

    return old;
}

int Scene_drawRegistered(int uid, int x, int y) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (_registeredScenes == 0) return -1;
    if (_registeredScenes[uid] == 0) return -1;

    return Scene_draw(_registeredScenes[uid], x, y);
}

int Scene_switchToRegistered(int uid, int x, int y) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (_registeredScenes == 0) return -1;
    if (_registeredScenes[uid] == 0) return -1;

    return Scene_switchTo(_registeredScenes[uid], x, y);
}

int Scene_scroll(int uid, int dx, int dy) {
    if (uid < 0 || uid >= MAX_REGISTERED_SCENES) return -1;
    if (_registeredScenes == 0) return -1;
    if (_registeredScenes[uid] == 0) return -1;

    CmdFX_Scene* scene = _registeredScenes[uid];
    if (scene->x == -1 && scene->y == -1) return -1;

    int* bounds = _drawnSceneBounds[uid];
    if (bounds == 0) return -1;

    int x1 = clamp_i(bounds[0] + dx, 0, scene->width); bounds[0] = x1;
    int y1 = clamp_i(bounds[1] + dy, 0, scene->height); bounds[1] = y1;
    int x2 = clamp_i(bounds[2] + dx, 0, scene->width); bounds[2] = x2;
    int y2 = clamp_i(bounds[3] + dy, 0, scene->height); bounds[3] = y2;

    Scene_remove0(scene);
    Scene_draw0(scene, scene->x, scene->y, x1, y1, x2, y2);

    return 0;
}

void tickCmdFXSceneEngine() {
    if (_registeredScenes == 0) return;

    for (int i = 0; i < Scene_getRegisteredScenesCount(); i++) {
        CmdFX_Scene* scene = _registeredScenes[i];
        if (scene == 0) continue;
        if (scene->x == -1 && scene->y == -1) continue;

        int* bounds = _drawnSceneBounds[scene->uid];
        int x1, y1, x2, y2;
        if (bounds != 0) {
            x1 = bounds[0];
            y1 = bounds[1];
            x2 = bounds[2];
            y2 = bounds[3];
        } else {
            x1 = 0;
            y1 = 0;
            x2 = scene->width;
            y2 = scene->height;
        }

        Scene_draw0(scene, scene->x, scene->y, x1, y1, x2, y2);
    }
}

// Scene Data Manipulation

int Scene_setData(CmdFX_Scene* scene, char** data) {
    if (scene == 0) return -1;
    if (data == 0) return -1;

    int height = getCharArrayHeight(data);
    int width = getCharArrayWidth(data);

    if (height < 0 || width < 0) return -1;

    int oldWidth = scene->width;
    int oldHeight = scene->height;

    int cx = scene->x;
    int cy = scene->y;
    if (scene->x != -1 && scene->y != -1) {
        Scene_remove0(scene);
        scene->x = -1;
        scene->y = -1;
    }

    if (scene->data != 0) {
        for (int i = 0; i < oldHeight; i++) free(scene->data[i]);
        free(scene->data);
    }
    scene->data = data;

    if (scene->ansiData != 0) {
        char*** ansiCopy = String2DBuilder_create(width, height);
        if (ansiCopy == 0) return -1;

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                if (i < oldHeight && j < oldWidth) ansiCopy[i][j] = scene->ansiData[i][j];
        
        for (int i = 0; i < oldHeight; i++)
            for (int j = 0; j < oldWidth; j++)
                free(scene->ansiData[i][j]);
        free(scene->ansiData);
    }

    scene->width = width;
    scene->height = height;

    if (cx != -1 && cy != -1) {
        Scene_draw0(scene, cx, cy, 0, 0, width, height);
        scene->x = cx;
        scene->y = cy;
    }

    return 0;
}

int Scene_setChar(CmdFX_Scene* scene, int x, int y, char c) {
    if (scene == 0) return -1;
    if (scene->data == 0) return -1;
    if (x < 0 || y < 0) return -1;
    if (x >= scene->width || y >= scene->height) return -1;

    scene->data[y][x] = c;
    if (scene->x != -1 && scene->y != -1 && Scene_isOnTopAt(scene, scene->x + x, scene->y + y)) {
        Canvas_setCursor(scene->x + x, scene->y + y);
        putchar(c);
    }
    
    return 0;
}

int Scene_setAnsiData(CmdFX_Scene* scene, char*** ansiData) {
    if (scene == 0) return -1;
    if (ansiData == 0) return -1;

    int height = getStringArrayHeight(ansiData);
    int width = getStringArrayWidth(ansiData);

    if (height < 0 || width < 0) return -1;

    int oldWidth = scene->width;
    int oldHeight = scene->height;

    int cx = scene->x;
    int cy = scene->y;
    if (scene->x != -1 && scene->y != -1) {
        Scene_remove0(scene);
        scene->x = -1;
        scene->y = -1;
    }

    if (scene->ansiData != 0) {
        for (int i = 0; i < oldHeight; i++)
            for (int j = 0; j < oldWidth; j++)
                free(scene->ansiData[i][j]);
        free(scene->ansiData);
    }
    scene->ansiData = ansiData;

    scene->width = width;
    scene->height = height;

    if (cx != -1 && cy != -1) {
        Scene_draw0(scene, cx, cy, 0, 0, width, height);
        scene->x = cx;
        scene->y = cy;
    }

    return 0;
}

int Scene_appendAnsiData(CmdFX_Scene* scene, char*** ansiData) {
    if (scene == 0) return -1;
    if (ansiData == 0) return -1;

    int height = getStringArrayHeight(ansiData);
    int width = getStringArrayWidth(ansiData);

    if (height < 0 || width < 0) return -1;

    int oldWidth = scene->width;
    int oldHeight = scene->height;

    int cx = scene->x;
    int cy = scene->y;
    if (scene->x != -1 && scene->y != -1) {
        Scene_remove0(scene);
        scene->x = -1;
        scene->y = -1;
    }

    char*** ansiCopy = String2DBuilder_create(width, height);
    if (ansiCopy == 0) return -1;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (i < oldHeight && j < oldWidth) {
                int l = strlen(scene->ansiData[i][j]) + strlen(ansiData[i][j]) + 1;
                char* buf = malloc(l);
                if (buf == 0) return -1;

                snprintf(buf, l, "%s%s", scene->ansiData[i][j], ansiData[i][j]);
                ansiCopy[i][j] = buf;
            }
            else ansiCopy[i][j] = ansiData[i][j];
    
    for (int i = 0; i < oldHeight; i++)
        for (int j = 0; j < oldWidth; j++)
            free(scene->ansiData[i][j]);
    free(scene->ansiData);

    scene->ansiData = ansiCopy;

    scene->width = width;
    scene->height = height;

    if (cx != -1 && cy != -1) {
        Scene_draw0(scene, cx, cy, 0, 0, width, height);
        scene->x = cx;
        scene->y = cy;
    }

    return 0;
}

int Scene_setForeground(CmdFX_Scene* scene, int x, int y, int width, int height, int rgb) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;
    if (width < 1 || height < 1) return -1;
    if (x + width > scene->width || y + height > scene->height) return -1;

    int cx = scene->x;
    int cy = scene->y;
    if (cx != -1 && cy != -1) {
        Scene_remove0(scene);
        scene->x = -1;
        scene->y = -1;
    }

    for (int i = y; i < y + height; i++)
        for (int j = x; j < x + width; j++) {
            char* ansi = scene->ansiData[i][j];
            if (ansi != 0) {
                char* buf = malloc(22);
                if (buf == 0) return -1;

                snprintf(ansi, 22, "\033[38;2;%d;%d;%dm", rgb >> 16 & 0xFF, rgb >> 8 & 0xFF, rgb & 0xFF);
                scene->ansiData[i][j] = buf;
            }
        }

    if (cx != -1 && cy != -1) {
        Scene_draw0(scene, cx, cy, 0, 0, width, height);
        scene->x = cx;
        scene->y = cy;
    }

    return 0;
}

int Scene_setForegroundAll(CmdFX_Scene* scene, int rgb) {
    if (scene == 0) return -1;
    return Scene_setForeground(scene, 0, 0, scene->width, scene->height, rgb);
}

int Scene_setBackground(CmdFX_Scene* scene, int x, int y, int width, int height, int rgb) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;
    if (width < 1 || height < 1) return -1;
    if (x + width > scene->width || y + height > scene->height) return -1;

    int cx = scene->x;
    int cy = scene->y;
    if (cx != -1 && cy != -1) {
        Scene_remove0(scene);
        scene->x = -1;
        scene->y = -1;
    }

    for (int i = y; i < y + height; i++)
        for (int j = x; j < x + width; j++) {
            char* ansi = scene->ansiData[i][j];
            if (ansi != 0) {
                char* buf = malloc(22);
                if (buf == 0) return -1;

                snprintf(ansi, 22, "\033[48;2;%d;%d;%dm", rgb >> 16 & 0xFF, rgb >> 8 & 0xFF, rgb & 0xFF);
                scene->ansiData[i][j] = buf;
            }
        }

    if (cx != -1 && cy != -1) {
        Scene_draw0(scene, cx, cy, 0, 0, width, height);
        scene->x = cx;
        scene->y = cy;
    }

    return 0;
}

int Scene_setBackgroundAll(CmdFX_Scene* scene, int rgb) {
    if (scene == 0) return -1;
    return Scene_setBackground(scene, 0, 0, scene->width, scene->height, rgb);
}