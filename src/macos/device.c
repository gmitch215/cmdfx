#include <stdio.h>

#include <ApplicationServices/ApplicationServices.h>

#include "cmdfx/device.h"

int Device_getKeyboardKeyPressed() {
    for (int key = 0; key < 128; key++) {
        if (CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, key)) return key;
    }

    return -1;
}

int Device_getMouseButtonPressed() {
    if (CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonLeft)) return kCGMouseButtonLeft;
    if (CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonRight)) return kCGMouseButtonRight;
    if (CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter)) return kCGMouseButtonCenter;

    return -1;
}