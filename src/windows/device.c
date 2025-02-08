#include <stdio.h>
#include <windows.h>

#include "cmdfx/device.h"

int Device_getKeyboardKeyPressed() {
    for (int key = 8; key <= 255; key++) {
        if (GetAsyncKeyState(key) & 0x8000) return key;
    }
    return -1;
}

int Device_getMouseButtonPressed() {
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) return VK_LBUTTON;
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) return VK_RBUTTON;
    if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) return VK_MBUTTON;

    return -1;
}