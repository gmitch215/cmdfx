#include <stdio.h>
#include <stdlib.h>

#include "../test.h"
#include "cmdfx/core/builder.h"
#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/engine.h"
#include "cmdfx/physics/motion.h"

static void tickAll(void) {
    CmdFX_Sprite** modified = Engine_tick();
    if (modified != 0) {
        for (int i = 0; modified[i] != 0; i++) Engine_applyMotion(modified[i]);
        free(modified);
    }
}

// strict (area) overlap, excluding mere edge adjacency
static int penetrating(CmdFX_Sprite* a, CmdFX_Sprite* b) {
    int ox = a->x < b->x + b->width && b->x < a->x + a->width;
    int oy = a->y < b->y + b->height && b->y < a->y + a->height;
    return ox && oy;
}

// runs a deterministic head-on approach between two 5x5 blocks and reports
// whether they touched, ever interpenetrated, and bounced apart
static void scenario(
    double v1, double v2, int* touched, int* penetrated, int* bounced, int* s1x,
    int* s2x
) {
    Engine_setForceOfGravity(0);
    Engine_setGroundY(0);
    Engine_setDefaultFrictionCoefficient(0);

    CmdFX_Sprite* s1 =
        Sprite_create(Char2DBuilder_createFilled(5, 5, '#'), 0, 0);
    CmdFX_Sprite* s2 =
        Sprite_create(Char2DBuilder_createFilled(5, 5, '@'), 0, 0);
    Sprite_draw(2, 20, s1);
    Sprite_draw(40, 20, s2);
    Sprite_setVelocityX(s1, v1);
    Sprite_setVelocityX(s2, v2);

    *touched = 0;
    *penetrated = 0;
    *bounced = 0;
    for (int t = 0; t < 30; t++) {
        tickAll();
        if (Sprite_isColliding(s1, s2)) *touched = 1;
        if (penetrating(s1, s2)) *penetrated = 1;
        if (Sprite_getVelocityX(s1) < 0 && Sprite_getVelocityX(s2) > 0)
            *bounced = 1;
    }

    *s1x = s1->x;
    *s2x = s2->x;

    Sprite_free(s1);
    Sprite_free(s2);
    Engine_cleanup();
}

int main() {
    int r = 0;
    int touched, penetrated, bounced, s1x, s2x;

    // slow head-on: contact, no interpenetration, bounce apart, no crossing
    scenario(2.0, -2.0, &touched, &penetrated, &bounced, &s1x, &s2x);
    r |= assertTrue(touched);
    r |= assertFalse(penetrated);
    r |= assertTrue(bounced);
    r |= assertLessThan(s1x, s2x);

    // fast head-on stresses tunneling; conservative advancement must still
    // stop them flush and keep '#' on the left of '@'
    scenario(12.0, -9.0, &touched, &penetrated, &bounced, &s1x, &s2x);
    r |= assertTrue(touched);
    r |= assertFalse(penetrated);
    r |= assertTrue(bounced);
    r |= assertLessThan(s1x, s2x);

    // determinism: identical inputs must yield identical final positions
    int t2, p2, b2, ax, bx;
    scenario(12.0, -9.0, &t2, &p2, &b2, &ax, &bx);
    r |= assertEquals(ax, s1x);
    r |= assertEquals(bx, s2x);

    return r;
}
