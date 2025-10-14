#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#include "cmdfx/core/device.h"

bool* Device_getKeyboardKeysPressed() {
    bool* keys = (bool*) calloc(256, sizeof(bool));

    for (int key = 8; key < 256; key++) {
        if (GetAsyncKeyState(key) & 0x8000) {
            keys[key] = 1;
        }
    }

    return keys;
}

char Device_fromKeyCode(int keyCode) {
    BYTE keyboardState[256] = {0};
    WCHAR unicodeChar = 0;

    if (GetKeyboardState(keyboardState)) {
        UINT scanCode = MapVirtualKey(keyCode, MAPVK_VK_TO_VSC);
        if (ToUnicode(keyCode, scanCode, keyboardState, &unicodeChar, 1, 0) >
            0) {
            return (char) unicodeChar;
        }
    }
    return '\0';
}

bool* Device_getMouseButtonsPressed() {
    bool* buttons = (bool*) calloc(3, sizeof(bool));

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) buttons[0] = 1;
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) buttons[1] = 1;
    if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) buttons[2] = 1;

    return buttons;
}