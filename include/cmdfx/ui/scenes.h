/**
 * @file scenes.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief UI extensions for Scenes in CmdFX
 * @version 0.2.0
 * @date 2025-04-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include "cmdfx/core/scenes.h"
#include "cmdfx/ui/button.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The maximum number of buttons per scene.
 *
 * This is the maximum number of buttons that can be added to a scene. If
 * this limit is reached, no more buttons can be added to the scene.
 */
#define MAX_BUTTONS_PER_SCENE 128

/**
 * @brief Gets the buttons of a scene.
 *
 * This function gets the buttons of a scene. The buttons are stored in the
 * scene and can be used to interact with the scene. The scene must be
 * registered in order to add buttons to it.
 *
 * @param uid The unique identifier of the scene.
 * @return A pointer to an array of pointers to the buttons in the scene.
 */
CmdFX_Button** Scene_getButtons(int uid);

/**
 * @brief Gets the number of buttons in a scene.
 *
 * This function gets the number of buttons in a scene. The buttons are
 * stored in the scene and can be used to interact with the scene. The scene
 * must be registered in order to add buttons to it.
 *
 * @param uid The unique identifier of the scene.
 * @return The number of buttons in the scene.
 */
int Scene_getButtonsCount(int uid);

/**
 * @brief Gets the coordinates of a button in a scene.
 *
 * This function gets the coordinates of a button in a scene. The
 * coordinates are stored in the scene and can be used to interact with the
 * scene.
 *
 * The array returned is a pointer to an array of two integers, where X is
 * the first element and Y is the second element. The coordinates are
 * relative to the scene. The array will be freed when the scene is freed,
 * so it should not be freed by the caller. If the button is not found, this
 * function returns `NULL`.
 *
 * @param uid The unique identifier of the scene.
 * @param button The button to get the coordinates of.
 * @return A pointer to an array of two integers representing the
 * coordinates of the button.
 */
int* Scene_getButtonCoordinates(int uid, CmdFX_Button* button);

/**
 * @brief Adds a button to a scene.
 *
 * This function adds a button to a scene. The button is stored in the scene
 * and can be used to interact with the scene. The scene must be registered
 * in order to add buttons to it.
 *
 * @param uid The unique identifier of the scene.
 * @param button The button to add to the scene.
 * @param x The x-coordinate of the button.
 * @param y The y-coordinate of the button.
 * @return `0` if the button was added successfully, or `-1` if an error
 * occurred.
 */
int Scene_addButton(int uid, CmdFX_Button* button, int x, int y);

/**
 * @brief Removes a button from a scene.
 *
 * This function removes a button from a scene. The button is removed from
 * the scene and can no longer be used to interact with the scene. The scene
 * must be registered in order to remove buttons from it.
 *
 * @param uid The unique identifier of the scene.
 * @param button The button to remove from the scene.
 * @return `0` if the button was removed successfully, or `-1` if an error
 * occurred.
 */
int Scene_removeButton(int uid, CmdFX_Button* button);

/**
 * @brief Removes a button from a scene at the specified coordinates.
 *
 * This function removes a button from a scene at the specified coordinates.
 * The button is removed from the scene and can no longer be used to
 * interact with the scene. The scene must be registered in order to remove
 * buttons from it. The button is not freed, only the array of buttons is
 * freed. The button is removed from the scene and can no longer be used to
 * interact with the scene.
 *
 * @param uid The unique identifier of the scene.
 * @param x The x-coordinate of the button to remove.
 * @param y The y-coordinate of the button to remove.
 * @return `0` if the button was removed successfully, or `-1` if an error
 * occurred.
 */
int Scene_removeButtonAt(int uid, int x, int y);

/**
 * @brief Removes all buttons from a scene.
 *
 * This function removes all buttons from a scene. The buttons are removed
 * from the scene and can no longer be used to interact with the scene, and
 * the array of buttons is freed. The scene must be registered in order to
 * remove buttons from it. The buttons are not freed, only the array of
 * buttons.
 *
 * @param uid The unique identifier of the scene.
 * @return `0` if all buttons were removed successfully, or `-1` if an error
 * occurred.
 */
int Scene_removeAllButtons(int uid);

#ifdef __cplusplus
}
#endif