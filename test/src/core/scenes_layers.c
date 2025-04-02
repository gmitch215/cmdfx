#include <stdio.h>

#include "cmdfx/core/builder.h"
#include "cmdfx/core/scenes.h"
#include "../test.h"

int main() {
    int r = 0;

    CmdFX_Scene* scene1 = Scene_createFilled(5, 5, '#', 0, 0);
    CmdFX_Scene* scene2 = Scene_createFilled(5, 5, '@', 0, 1);
    CmdFX_Scene* scene3 = Scene_createFilled(5, 5, '%', 0, 2);
    CmdFX_Scene* scene4 = Scene_createFilled(5, 5, '&', 0, 3);

    r |= assertNotNull(scene1);
    r |= assertNotNull(scene2);
    r |= assertNotNull(scene3);
    r |= assertNotNull(scene4);

    Scene_draw(scene1, 0, 0);
    Scene_draw(scene2, 1, 1);
    Scene_draw(scene3, 2, 2);
    Scene_draw(scene4, 3, 3);

    r |= assertEquals(scene1->x, 0);
    r |= assertEquals(scene1->y, 0);
    r |= assertEquals(scene2->x, 1);
    r |= assertEquals(scene2->y, 1);
    r |= assertEquals(scene3->x, 2);
    r |= assertEquals(scene3->y, 2);
    r |= assertEquals(scene4->x, 3);
    r |= assertEquals(scene4->y, 3);

    r |= assertPointersMatch(Scene_getSceneAt(0, 0), scene1);
    r |= assertPointersMatch(Scene_getSceneAt(1, 1), scene2);
    r |= assertPointersMatch(Scene_getSceneAt(2, 2), scene3);
    r |= assertPointersMatch(Scene_getSceneAt(3, 3), scene4);

    r |= assertTrue(Scene_isOnTop(scene1));
    r |= assertTrue(Scene_isOnTop(scene2));
    r |= assertTrue(Scene_isOnTop(scene3));
    r |= assertTrue(Scene_isOnTop(scene4));

    r |= assertTrue(Scene_isOnBottomAt(scene1, 4, 4));
    r |= assertTrue(Scene_isOnBottomAt(scene2, 5, 5));
    r |= assertTrue(Scene_isOnBottomAt(scene3, 6, 6));
    r |= assertTrue(Scene_isOnBottomAt(scene4, 7, 7));

    Scene_free(scene1);
    Scene_free(scene2);
    Scene_free(scene3);
    Scene_free(scene4);

    return r;
}