#include <stdio.h>
#include <stdlib.h>
#include <cmdfx.h>

#include "../test.h"

int onKey(CmdFX_Event* event) {
    CmdFX_KeyEvent* keyEvent = (CmdFX_KeyEvent*) event->data;
    printf("Key state: '%c' (%d), %d\n", keyEvent->keyChar, keyEvent->keyCode, keyEvent->state);

    if (keyEvent->keyChar == 'q' || keyEvent->keyChar == 'Q') {
        Screen_setEchoEnabled(1);
        Screen_setLineBuffered(1);
        exit(0);
    }

    return 0;
}

int main() {
    int r = 0;

    Canvas_clearScreen();

    printf("P-Echo Enabled: %d\n", Screen_isEchoEnabled());
    printf("P-Line Buffered: %d\n", Screen_isLineBuffered());

    Screen_setEchoEnabled(0);
    Screen_setLineBuffered(0);

    printf("Echo Enabled: %d\n", Screen_isEchoEnabled());
    printf("Line Buffered: %d\n", Screen_isLineBuffered());
    printf("Press 'q' to quit\n");

    addCmdFXEventListener(CMDFX_EVENT_KEY, onKey);

    while (1) {
        // Do nothing
    }
}