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

#include <vector>

namespace CmdFX {
    /**
     * @brief C++ wrapper for the CmdFX Device API.
     */
    namespace Device {
        /**
         * @brief Gets a vector of booleans representing the state of the keyboard keys.
         * 
         * This function retrieves the state of the keyboard keys and returns a vector
         * of booleans indicating whether each key is pressed or not. The vector
         * will contain 256 elements, corresponding to the computer's keyboard layout.
         * 
         * @return A vector of booleans representing the state of each keyboard key.
         */
        std::vector<bool> getKeyboardKeysPressed() { 
            std::vector<bool> keysPressed;
            keysPressed.reserve(256);

            bool* arr = Device_getKeyboardKeysPressed();
            if (arr == nullptr) return keysPressed;

            for (int i = 0; i < 256; i++) {
                keysPressed.push_back(arr[i]);
            }
            
            return keysPressed;
        }
        char fromKeyCode(int keyCode) { return Device_fromKeyCode(keyCode); }

        /**
         * @brief Gets a vector of mouse buttons that are currently pressed.
         * 
         * This function retrieves the state of the mouse buttons and returns a vector
         * of booleans indicating whether each button is pressed or not. The vector
         * will contain three elements, corresponding to the left, middle, and right mouse buttons.
         * 
         * @return A vector of booleans representing the state of each mouse button.
         */
        std::vector<bool> getMouseButtonsPressed() { 
            std::vector<bool> buttonsPressed;
            buttonsPressed.reserve(3);

            bool* arr = Device_getMouseButtonsPressed();
            if (arr == nullptr) return buttonsPressed;

            for (int i = 0; i < 3; i++) {
                buttonsPressed.push_back(arr[i]);
            }
            
            return buttonsPressed;
        }
    };
}