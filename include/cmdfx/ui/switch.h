/**
 * @file switch.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Switch extensions for the CmdFX Button API
 * @version 0.2.0
 * @date 2025-04-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include <stdbool.h>

#include "cmdfx/ui/button.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a CmdFX switch button.
 * 
 * Switch buttons are a type of button that can be toggled on and off. They
 * also can change costumes based on their state. Their `extra` field is a
 * pointer to a boolean that indicates the state of the switch.
 */
#define CMDFX_BUTTON_TYPE_SWITCH 1

/**
 * @brief Creates a switch button.
 * 
 * This function creates a switch button with the given sprite and callback function.
 * 
 * @param sprite 
 * @param callback 
 * @param state 
 * @return CmdFX_Button* 
 */
CmdFX_Button* Button_createSwitch(CmdFX_Sprite* sprite, CmdFX_ButtonCallback callback, bool state);

/**
 * @brief Creates a switch button with the given on and off states.
 * 
 * This function creates a switch button with the given on and off states. The button
 * will be created with the given sprite and callback function.
 * 
 * @param on The string to display when the button is on.
 * @param off The string to display when the button is off.
 * @param ansiOn The ANSI codes to use when the button is on.
 * @param ansiOff The ANSI codes to use when the button is off.
 * @param callback The callback function to call when the button is clicked.
 * @param state The initial state of the button (0 = off, 1 = on).
 * @return CmdFX_Button* A pointer to the newly created switch button.
 */
CmdFX_Button* Button_createSwitchWith(char** on, char** off, char*** ansiOn, char*** ansiOff, CmdFX_ButtonCallback callback, bool state);

/**
 * @brief Gets the state of the switch button.
 * 
 * This function gets the state of the switch button. The state is stored in
 * the `extra` field of the button, which is a pointer to a boolean.
 * 
 * @param button The button to get the state of.
 * @return bool The state of the switch button (0 = off, 1 = on).
 */
bool Switch_getState(CmdFX_Button* button);

/**
 * @brief Sets the state of the switch button.
 * 
 * This function sets the state of the switch button. The state is stored in
 * the `extra` field of the button, which is a pointer to a boolean.
 * 
 * @param button The button to set the state of.
 * @param state The new state of the switch button (0 = off, 1 = on).
 * @return int 0 if successful, -1 if an error occurred.
 */
int Switch_setState(CmdFX_Button* button, bool state);

/**
 * @brief Toggles the state of the switch button.
 * 
 * This function toggles the state of the switch button. The state is stored in
 * the `extra` field of the button, which is a pointer to a boolean.
 * 
 * @param button The button to toggle the state of.
 * @return int 0 if successful, -1 if an error occurred.
 */
int Switch_toggleState(CmdFX_Button* button);

#ifdef __cplusplus
}
#endif