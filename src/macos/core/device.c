#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include "cmdfx/core/device.h"

bool* Device_getKeyboardKeysPressed() {
    bool* keys = (bool*) calloc(256, sizeof(bool)); 

    for (int key = 0; key < 256; key++) {
        if (CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, key)) {
            keys[key] = 1;
        }
    }

    return keys;
}

char Device_fromKeyCode(int keyCode) {
    TISInputSourceRef source = TISCopyCurrentKeyboardLayoutInputSource();
    if (!source) return '\0';

    CFDataRef layoutData = (CFDataRef) TISGetInputSourceProperty(source, kTISPropertyUnicodeKeyLayoutData);
    if (!layoutData) return '\0';

    const UCKeyboardLayout* keyboardLayout = (const UCKeyboardLayout*) CFDataGetBytePtr(layoutData);
    if (!keyboardLayout) return '\0';

    UInt32 deadKeyState = 0;
    UniChar unicode = 0;
    UniCharCount length = 0;

    OSStatus status = UCKeyTranslate(keyboardLayout, keyCode, kUCKeyActionDown, 0, LMGetKbdType(), kUCKeyTranslateNoDeadKeysBit, &deadKeyState, 1, &length, &unicode);
    if (status != noErr) return '\0';

    return (char) unicode;
}

bool* Device_getMouseButtonsPressed() {
    bool* buttons = (bool*) calloc(3, sizeof(bool));

    if (CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonLeft)) buttons[0] = 1;
    if (CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonRight)) buttons[1] = 1;
    if (CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter)) buttons[2] = 1;

    return buttons;
}