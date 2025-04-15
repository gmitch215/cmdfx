/**
 * @file button.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Button declarations for the CmdFX UI Library
 * @version 0.2.0
 * @date 2025-04-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include "cmdfx/core/sprites.h"
#include "cmdfx/core/events.h"

#ifdef __cplusplus
extern "C" {
#endif

// use void pointer becuase not declared yet

/**
 * @brief The callback function to be called when the button is clicked.
 * 
 * This function is called when the button is clicked. The function
 * should take a pointer to the button and a pointer to the mouse event
 * as parameters. The function should return nothing.
 * 
 * @param button The button that was clicked.
 * @param event The mouse event that triggered the callback.
 * @param time The time at which the event occurred.
 */
typedef void (*CmdFX_ButtonCallback)(void* button, CmdFX_MouseEvent* event, unsigned long long time);

/**
 * @brief Represents a CmdFX button.
 * 
 * Buttons are objects that can be clicked on by the user and perform
 * an action when clicked. Buttons have a sprite for visual implementation
 * while also including a callback function to be called when the button is clicked.
 */
typedef struct CmdFX_Button {
    /**
     * @brief The sprite associated with this button.
     * 
     * This sprite is used to render the button on the screen. All buttons
     * are static sprites, meaning they are not affected by the physics engine.
     */
    CmdFX_Sprite* sprite;
    /**
     * @brief The X position of the button.
     * 
     * This is the same as `sprite->x`, but is used to determine the position of the
     * button on the screen. This value is set when the button is drawn.
     */
    int x;
    /**
     * @brief The Y position of the button.
     * 
     * This is the same as `sprite->y`, but is used to determine the position of the
     * button on the screen. This value is set when the button is drawn.
     */
    int y;
    /**
     * @brief The unique ID of the button.
     * 
     * This is assigned when it is on the screen. It is used to identify the
     * button when editing or removing it. This value is identical to the
     * value of `sprite->id - 1`, but will not change if the sprite is removed.
     * 
     * If the button is removed or not on the screen, this value will be `-1`.
     */
    int id;
    /**
     * @brief The callback function to be called when the button is clicked.
     * 
     * This function is called when the button is clicked. The function
     * should take a pointer to the button and a pointer to the mouse event
     * as parameters. The function should return nothing.
     */
    CmdFX_ButtonCallback* callback;
} CmdFX_Button;

/**
 * @brief Gets an array of registered buttons.
 * 
 * This function returns the number of buttons that are currently registered
 * with the UI manager. This includes all buttons that have been created and
 * added to the UI manager.
 * 
 * @return An array of pointers to the registered buttons on the terminal screen.
 */
CmdFX_Button** Canvas_getRegisteredButtons();

/**
 * @brief Gets the number of registered buttons.
 * 
 * This function returns the number of buttons that are currently registered
 * with the UI manager. This includes all buttons that have been created and
 * added to the UI manager.
 * 
 * @return int The number of registered buttons.
 */
int Canvas_getRegisteredButtonsCount();

/**
 * @brief Creates a new button.
 * 
 * This function creates a new button with the given sprite and callback function.
 * The button is not added to the UI manager, so it must be added manually.
 * 
 * @param sprite The sprite to use for the button.
 * @param callback The callback function to call when the button is clicked.
 * @return CmdFX_Button* A pointer to the newly created button.
 */
CmdFX_Button* Button_create(CmdFX_Sprite* sprite, CmdFX_ButtonCallback callback);

/**
 * @brief Frees the memory associated with a button.
 * 
 * This function frees the memory associated with the given button. The
 * button will no longer be valid after this function is called. The sprite
 * associated with the button is also freed.
 * 
 * @param button The button to free.
 */
void Button_free(CmdFX_Button* button);

/**
 * @brief Draws the button on the screen.
 * 
 * This function draws the button on the screen at the given position. The
 * button is drawn using the sprite associated with the button. The Z-index
 * of the button is set to the current Z-index of its sprite.
 * 
 * @param x The x position to draw the button at.
 * @param y The y position to draw the button at.
 * @param button The button to draw.
 * @return 0 if successful, else -1
 */
int Button_draw(int x, int y, CmdFX_Button* button);

/**
 * @brief Removes a button from the UI manager.
 * 
 * This function removes the given button from the UI manager. The button
 * will no longer be rendered on the screen and will not respond to mouse events.
 * 
 * @param button The button to remove.
 * @return 0 if successful, else -1
 */
int Button_remove(CmdFX_Button* button);

/**
 * @brief Checks if a button is hidden.
 * 
 * This function checks if the given button is hidden from the UI manager.
 * A button is considered hidden if it is not rendered on the screen and
 * does not respond to mouse events.
 * 
 * @param button The button to check.
 * @return 1 if the button is hidden, 0 otherwise.
 */
int Button_isHidden(CmdFX_Button* button);

/**
 * @brief Hides a button from the UI manager.
 * 
 * This function hides the given button from the UI manager. The button
 * will still be present and will respond to mouse events, but it will not
 * be rendered on the screen. This is useful for temporarily hiding a button
 * without removing it from the UI manager.
 * 
 * @param button The button to hide.
 * @return 0 if successful, else -1
 */
int Button_hide(CmdFX_Button* button);

/**
 * @brief Shows a button in the UI manager if it is already hidden.
 * 
 * This function shows the given button in the UI manager. The button
 * will be rendered on the screen and will respond to mouse events.
 * 
 * @param button The button to show.
 * @return 0 if successful, else -1
 */
int Button_show(CmdFX_Button* button);

/**
 * @brief Gets all buttons at the given position.
 * 
 * This function returns an array of all buttons at the given position.
 * The position is specified by the x and y coordinates. If there are no
 * buttons at the given position, this function returns NULL.
 * 
 * @param x The x position to check.
 * @param y The y position to check.
 * @return An array of pointers to the buttons at the given position, or NULL if no buttons are found.
 */
CmdFX_Button** Canvas_getAllButtonsAt(int x, int y);

/**
 * @brief Gets the button at the given position.
 * 
 * This function returns the button at the given position with the highest
 * z-index. The position is specified by the x and y coordinates. If there 
 * is no button at the given position, this function returns NULL.
 * 
 * @param x The x position to check.
 * @param y The y position to check.
 * @return A pointer to the button at the given position, or NULL if no button is found.
 */
CmdFX_Button* Canvas_getButtonAt(int x, int y);

#ifdef __cplusplus
}
#endif