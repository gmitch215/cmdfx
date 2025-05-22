/**
 * @file device.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the CmdFX Device API
 * @version 0.2.1
 * @date 2025-05-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

extern "C" {
    #include "cmdfx/core/device.h"
}

namespace CmdFX {
    /**
     * @brief C++ wrapper for the CmdFX Device API.
     */
    namespace Device {
        bool* getKeyboardKeysPressed() { return Device_getKeyboardKeysPressed(); }
        char fromKeyCode(int keyCode) { return Device_fromKeyCode(keyCode); }
        bool* getMouseButtonsPressed() { return Device_getMouseButtonsPressed(); }
    };
}