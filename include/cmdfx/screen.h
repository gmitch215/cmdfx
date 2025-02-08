/**
 * @file screen.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Screen API for controlling other things around the terminal.
 * @version 0.1.0
 * @date 2025-01-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Window API

/**
 * @brief Gets the title of the terminal window.
 * @return The title of the terminal window.
 */
const char* Window_getTitle();

/**
 * @brief Sets the title of the terminal window.
 * @param title The title to set.
 */
void Window_setTitle(const char* title);

/**
 * @brief Gets the size of the terminal window, and stores it in the width and height variables.
 * @param width The pointer to the width of the terminal window.
 * @param height The pointer to the height of the terminal window.
 */
void Window_getSize(int* width, int* height);

/**
 * @brief Sets the size of the terminal window.
 * @param width The width of the terminal window.
 * @param height The height of the terminal window.
 */
void Window_setSize(int width, int height);

// Screen API

/**
 * @brief Gets the size of the screen, and stores it in the width and height variables.
 * @param width The pointer to the width of the screen.
 * @param height The pointer to the height of the screen.
 */
void Screen_getSize(int* width, int* height);

/**
 * @brief Gets the position of the mouse, and stores it in the x and y variables.
 * 
 * This method gets the position of the mouse on the screen. The x and y variables will be
 * updated with the x and y position of the mouse.
 * 
 * This method will require administrator privileges on Linux.
 * 
 * @param x The pointer to the x position of the mouse.
 * @param y The pointer to the y position of the mouse.
 */
void Screen_getMousePos(int* x, int* y);

/**
 * @brief Gets whether or not echoing is enabled.
 * 
 * This method gets whether or not echoing is enabled on the terminal.
 * Echoing is when the terminal displays the characters that are typed.
 * If this is off, the terminal will not display the characters when
 * they are typed.
 * 
 * @return 1 if echoing is enabled, 0 if it is not.
 */
int Screen_isEchoEnabled();

/**
 * @brief Sets whether or not echoing is enabled.
 * 
 * @param enabled 1 to enable echoing, 0 to disable echoing.
 * @return 0 if successful, -1 if an error occurred.
 */
int Screen_setEchoEnabled(int enabled);

/**
 * @brief Gets whether or not line buffering is enabled.
 * 
 * This method gets whether or not line buffering is enabled on the terminal.
 * Line buffering is when the terminal waits for a newline character before
 * sending the input to the program. This must be disabled in order to listen
 * for individual key presses, otherwise events will only pass if the user
 * presses enter.
 * 
 * @return 1 if line buffering is enabled, 0 if it is not.
 */
int Screen_isLineBuffered();

/**
 * @brief Sets whether or not line buffering is enabled.
 * 
 * @param enabled 1 to enable line buffering, 0 to disable line buffering.
 * @return 0 if successful, -1 if an error occurred.
 */
int Screen_setLineBuffered(int enabled);

#ifdef __cplusplus
}
#endif