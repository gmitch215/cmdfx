#include <cmdfx.h>
#include <stdio.h>

#include "../test.h"

int onMouse(CmdFX_Event* event) {
    CmdFX_MouseEvent* keyEvent = (CmdFX_MouseEvent*) event->data;
    printf(
        "Mouse event: %d (state: %d) at (%d, %d)\n", keyEvent->button,
        keyEvent->state, keyEvent->x, keyEvent->y
    );
    return 0;
}

int main() {
    int r = 0;

    Canvas_clearScreen();
    addCmdFXEventListener(CMDFX_EVENT_MOUSE, onMouse);

    while (1) {
        // Do nothing
    }
}