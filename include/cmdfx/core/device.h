/**
 * @file device.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Device command functions
 * @version 0.1.1
 * @date 2025-02-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Gets the current keys being pressed.
     *
     * The size of the array is always 256, with each index corresponding to a
     * key code native to the operating system. If the key is being pressed, the
     * value at the index will be 1. If the key is not being pressed, the value
     * at the index will be 0.
     *
     * This is a non-blocking method. This method gets the current key being
     * pressed on the keyboard. If no key is being pressed, it will return -1.
     *
     * The array must be freed after use.
     *
     * **Preconditions:**
     *
     * On macOS, the application must be added to the "Input Monitoring"
     * permission in the "Security & Privacy" settings. Otherwise, only control
     * keys will be detected.
     *
     * On Linux, this method requires administrator privileges.
     *
     * @return The list of key code of the key being pressed.
     */
    bool* Device_getKeyboardKeysPressed();

    /**
     * @brief Converts a key code to a character.
     * @param keyCode The key code to convert.
     * @return The character representation of the key code.
     */
    char Device_fromKeyCode(int keyCode);

    /**
     * @brief Gets the current mouse button being pressed.
     *
     * This is a non-blocking method. This method gets the current mouse event.
     * If no mouse button is being pressed, all buttons will be 0.
     *
     * On most systems, the button values are as follows (according to index):
     * - 0: Left mouse button
     * - 1: Right mouse button
     * - 2: Middle mouse button
     *
     * The array must be freed after use.
     *
     * This method requires administrator privileges on Linux.
     *
     * @return The list of mouse button code of the button being pressed.
     */
    bool* Device_getMouseButtonsPressed();

#ifdef __cplusplus
}
#endif