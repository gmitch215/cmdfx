#include <cmdfx.h>
#include <stdio.h>

#include "../test.h"

// Manual, interactive check of the real curses backend. Run this in an actual
// terminal: it should render colored + styled text and shapes (proving the SGR
// -> curses attribute translation works) and quit on 'q' (proving event-driven
// input mapping works). Not run in CI (no attached TTY).

static int done = 0;

int onKey(CmdFX_Event* event) {
    CmdFX_KeyEvent* key = (CmdFX_KeyEvent*) event->data;
    if (key->keyChar == 'q' || key->keyChar == 'Q') done = 1;
    return 0;
}

int onResize(CmdFX_Event* event) {
    CmdFX_ResizeEvent* resize = (CmdFX_ResizeEvent*) event->data;
    Canvas_setCursor(2, 14);
    char buf[64];
    snprintf(
        buf, sizeof(buf), "resized to %dx%d   ", resize->newWidth,
        resize->newHeight
    );
    Canvas_drawText(2, 14, buf);
    return 0;
}

int main() {
    Canvas_clearScreen();
    Canvas_hideCursor();

    int width, height;
    Window_getSize(&width, &height);

    Canvas_setForeground(0xFF3030);
    Canvas_drawText(2, 2, "cmdfx curses backend -- manual check");
    Canvas_resetFormat();

    Canvas_setForeground(0x30FF30);
    Canvas_enableBold();
    Canvas_drawText(2, 4, "green + bold via curses attributes");
    Canvas_resetFormat();

    Canvas_setBackground(0x000080);
    Canvas_fillRect(2, 6, 24, 3, ' ');
    Canvas_resetFormat();

    char info[64];
    snprintf(info, sizeof(info), "terminal size: %dx%d", width, height);
    Canvas_drawText(2, 10, info);
    Canvas_drawText(2, 12, "resize the window, then press q to quit");

    addCmdFXEventListener(CMDFX_EVENT_KEY, onKey);
    addCmdFXEventListener(CMDFX_EVENT_RESIZE, onResize);
    beginCmdFXEventLoop();

    while (!done) sleepMillis(50);

    endCmdFXEventLoop();
    Canvas_showCursor();
    Canvas_clearScreen();
    return 0;
}
