#include <cmdfx.h>
#include <stdio.h>

#include "../test.h"

int onResize(CmdFX_Event* event) {
    CmdFX_ResizeEvent* resizeEvent = (CmdFX_ResizeEvent*) event->data;
    printf(
        "Terminal resized from %dx%d to %dx%d\n", resizeEvent->prevWidth,
        resizeEvent->prevHeight, resizeEvent->newWidth, resizeEvent->newHeight
    );
    return 0;
}

int main() {
    int r = 0;

    Canvas_clearScreen();
    addCmdFXEventListener(CMDFX_EVENT_RESIZE, onResize);

    while (1) {
        // Do nothing
    }
}