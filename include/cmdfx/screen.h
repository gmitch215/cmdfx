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
 * @brief Gets the RGB value of a pixel on the screen.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The RGB value of the pixel.
 */
int Screen_getRGB(int x, int y);

#ifdef __cplusplus
}
#endif