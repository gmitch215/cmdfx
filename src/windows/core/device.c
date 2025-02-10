#include <stdio.h>
#include <windows.h>

#include "cmdfx/core/device.h"

int* Device_getKeyboardKeysPressed() {
    int* keys = (int*) calloc(256, sizeof(int)); 

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
        if (ToUnicode(keyCode, scanCode, keyboardState, &unicodeChar, 1, 0) > 0) {
            return (char)unicodeChar;
        }
    }
    return '\0';
}

int* Device_getMouseButtonsPressed() {
    int* buttons = (int*) calloc(3, sizeof(int));

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) buttons[0] = 1;
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) buttons[1] = 1;
    if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) buttons[2] = 1;

    return buttons;
}