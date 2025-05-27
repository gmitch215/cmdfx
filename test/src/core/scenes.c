#include <stdio.h>

#include "cmdfx/core/builder.h"
#include "cmdfx/core/scenes.h"
#include "../test.h"

int main() {
    int r = 0;

    char** data = Char2DBuilder_createFilled(10, 5, '#');
    data[3][3] = '&';
    data[3][4] = '@';
    CmdFX_Scene* scene = Scene_createFromData(data, 0);

    r |= assertEquals(scene->width, 10);
    r |= assertEquals(scene->height, 5);
    r |= assertEquals(scene->x, -1);
    r |= assertEquals(scene->y, -1);

    r |= assertCharArraysMatch(scene->data, data);
    r |= assertPointersMatch(scene->ansiData, 0);
    r |= assertEquals(scene->z, 0);

    Scene_draw(scene, 0, 0);

    r |= assertEquals(scene->x, 0);
    r |= assertEquals(scene->y, 0);

    Scene_remove(scene);
    Scene_draw(scene, 10, 10);
    scene->z = 2;

    r |= assertEquals(scene->x, 10);
    r |= assertEquals(scene->y, 10);
    r |= assertEquals(scene->z, 2);

    r |= assertPointersMatch(Scene_getSceneAt(10, 10), scene);
    r |= assertPointersMatch(Scene_getSceneAt(10, 11), scene);
    r |= assertPointersMatch(Scene_getSceneAt(10 + scene->width, 10 + scene->height), 0);

    r |= assertEquals(Canvas_getDrawnScenesCount(), 1);
    r |= assertPointersMatch(Canvas_getDrawnScenes()[0], scene);

    Scene_free(scene);

    return r;
}