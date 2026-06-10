#include <stdbool.h>
#include <stdlib.h>

#include "cmdfx/core/device.h"

bool* Device_getKeyboardKeysPressed() {
    return calloc(256, sizeof(bool));
}

char Device_fromKeyCode(int keyCode) {
    (void) keyCode;
    return 0;
}

bool* Device_getMouseButtonsPressed() {
    return calloc(3, sizeof(bool));
}
