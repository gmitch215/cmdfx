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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Gets the current key being pressed, or -1 if no key is being pressed.
 * 
 * This is a non-blocking method. This method gets the current key being pressed 
 * on the keyboard. If no key is being pressed, it will return -1.
 * 
 * This method requires administrator privileges on Linux.
 * 
 * @return The key code of the key being pressed, or -1 if no key is being pressed.
 */
int Device_getKeyboardKeyPressed();

/**
 * @brief Gets the current mouse button being pressed, or -1 if no button is being pressed.
 * 
 * This is a non-blocking method. This method gets the current mouse event. If no mouse button 
 * is being pressed, it will return -1.
 * 
 * This method requires administrator privileges on Linux.
 * 
 * @return The mouse event struct.
 */
int Device_getMouseButtonPressed();

#ifdef __cplusplus
}
#endif