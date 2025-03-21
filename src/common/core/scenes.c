#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/core/canvas.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/scenes.h"

CmdFX_Scene** _drawnScenes = 0;
int _drawnScenesCount = 0;

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

CmdFX_Scene* Scene_createFromData(char** data, char*** ansiData) {
    if (data == 0) return 0;

    int height = getArrayHeight(data);
    int width = getArrayWidth(data);

    if (height < 1 || width < 1) return 0;

    CmdFX_Scene* scene = (CmdFX_Scene*) malloc(sizeof(CmdFX_Scene));
    if (scene == 0) return 0;

    scene->uid = -1;
    scene->width = width;
    scene->height = height;
    scene->data = data;

    if (ansiData != 0) {
        int ansiHeight = getAnsiArrayHeight(ansiData);
        int ansiWidth = getAnsiArrayWidth(ansiData);
    
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

void Scene_draw0(CmdFX_Scene* scene, int x, int y) {
    int width = scene->width;
    int height = scene->height;
    
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            char c = scene->data[i][j];
            if (c == 0) break;
            if (c == ' ') continue;

            int cx = x + j;
            int cy = y + i;

            if (!Scene_isOnTopAt(scene, cx, cy)) continue;

            Canvas_setCursor(cx, cy);
            putchar(c);

            if (scene->ansiData != 0) {
                char* ansi = scene->ansiData[i][j];
                if (ansi != 0) printf("%s", ansi);
            }
        }
}

int Scene_draw(CmdFX_Scene* scene, int x, int y) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;

    if (scene->x != -1 && scene->y != -1) Scene_remove(scene);
    scene->x = x;
    scene->y = y;
    Scene_draw0(scene, x, y);
    
    if (_drawnScenes == 0) {
        _drawnScenes = (CmdFX_Scene**) malloc(sizeof(CmdFX_Scene*));
    } else {
        _drawnScenes = (CmdFX_Scene**) realloc(_drawnScenes, sizeof(CmdFX_Scene*) * (_drawnScenesCount + 1));
    }

    _drawnScenes[_drawnScenesCount] = scene;
    _drawnScenesCount++;

    return 0;
}

CmdFX_Scene* Scene_getSceneAt(int x, int y) {
    if (x < 0 || y < 0) return 0;
    if (_drawnScenes == 0) return 0;
    if (_drawnScenesCount == 0) return 0;
    if (x >= Canvas_getWidth() || y >= Canvas_getHeight()) return 0;
    if (x >= Canvas_getWidth() || y >= Canvas_getHeight()) return 0;

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

    CmdFX_Scene* bottom = Scene_getSceneAt(scene->x, scene->y);
    if (bottom == 0) return 0;

    return bottom == scene ? 1 : 0;
}

int Scene_isOnBottomAt(CmdFX_Scene* scene, int x, int y) {
    if (scene == 0) return -1;
    if (x < 0 || y < 0) return -1;

    CmdFX_Scene* bottom = Scene_getSceneAt(x, y);
    if (bottom == 0) return 0;

    return bottom == scene ? 1 : 0;
}

void Scene_remove0(CmdFX_Scene* scene) {
    int width = scene->width;
    int height = scene->height;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            Canvas_setCursor(scene->x + j, scene->y + i);
            putchar(' ');
            printf("\033[0m");
        }
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

void tickCmdFXSceneEngine() {
    if (_registeredScenes == 0) return;

    for (int i = 0; i < Scene_getRegisteredScenesCount(); i++) {
        CmdFX_Scene* scene = _registeredScenes[i];
        if (scene == 0) continue;
        if (scene->x == -1 && scene->y == -1) continue;

        Scene_draw0(scene, scene->x, scene->y);
    }
}

// Scene Data Manipulation

int Scene_setData(CmdFX_Scene* scene, char** data) {
    if (scene == 0) return -1;
    if (data == 0) return -1;

    int height = getArrayHeight(data);
    int width = getArrayWidth(data);

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
        Scene_draw0(scene, cx, cy);
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

    int height = getAnsiArrayHeight(ansiData);
    int width = getAnsiArrayWidth(ansiData);

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
        Scene_draw0(scene, cx, cy);
        scene->x = cx;
        scene->y = cy;
    }

    return 0;
}

int Scene_appendAnsiData(CmdFX_Scene* scene, char*** ansiData) {
    if (scene == 0) return -1;
    if (ansiData == 0) return -1;

    int height = getAnsiArrayHeight(ansiData);
    int width = getAnsiArrayWidth(ansiData);

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
        Scene_draw0(scene, cx, cy);
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
        Scene_draw0(scene, cx, cy);
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
        Scene_draw0(scene, cx, cy);
        scene->x = cx;
        scene->y = cy;
    }

    return 0;
}

int Scene_setBackgroundAll(CmdFX_Scene* scene, int rgb) {
    if (scene == 0) return -1;
    return Scene_setBackground(scene, 0, 0, scene->width, scene->height, rgb);
}