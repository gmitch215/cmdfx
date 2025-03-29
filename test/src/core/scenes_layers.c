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

    r |= assert(scene1 != 0);
    r |= assert(scene2 != 0);
    r |= assert(scene3 != 0);
    r |= assert(scene4 != 0);

    Scene_draw(scene1, 0, 0);
    Scene_draw(scene2, 1, 1);
    Scene_draw(scene3, 2, 2);
    Scene_draw(scene4, 3, 3);

    r |= assert(scene1->x == 0);
    r |= assert(scene1->y == 0);
    r |= assert(scene2->x == 1);
    r |= assert(scene2->y == 1);
    r |= assert(scene3->x == 2);
    r |= assert(scene3->y == 2);
    r |= assert(scene4->x == 3);
    r |= assert(scene4->y == 3);

    r |= assert(Scene_getSceneAt(0, 0) == scene1);
    r |= assert(Scene_getSceneAt(1, 1) == scene2);
    r |= assert(Scene_getSceneAt(2, 2) == scene3);
    r |= assert(Scene_getSceneAt(3, 3) == scene4);

    r |= assert(Scene_isOnTop(scene1));
    r |= assert(Scene_isOnTop(scene2));
    r |= assert(Scene_isOnTop(scene3));
    r |= assert(Scene_isOnTop(scene4));

    r |= assert(Scene_isOnBottomAt(scene1, 4, 4));
    r |= assert(Scene_isOnBottomAt(scene2, 5, 5));
    r |= assert(Scene_isOnBottomAt(scene3, 6, 6));
    r |= assert(Scene_isOnBottomAt(scene4, 7, 7));

    Scene_free(scene1);
    Scene_free(scene2);
    Scene_free(scene3);
    Scene_free(scene4);

    return r;
}