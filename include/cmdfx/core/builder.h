/**
 * @file builder.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Builder API for building 2D Character Arrays.
 * @version 0.1.0
 * @date 2025-01-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "cmdfx/core/sprites.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma region Character Builder

// Utility Functions - General (Character)

/**
 * @brief Gets the width of a 2D Character Array.
 * @param array The 2D Character Array.
 * @return The width of the array.
 */
int getArrayWidth(char** array);

/**
 * @brief Gets the height of a 2D Character Array.
 * @param array The 2D Character Array.
 * @return The height of the array.
 */
int getArrayHeight(char** array);

/**
 * @brief Creates a copy of a 2D Character Array.
 * @param array The 2D Character Array.
 * @return A copy of the array.
 */
char** createArrayCopy(char** array);

// Core Functions (Character)

/**
 * @brief Creates a 2D Character Array with a specific width and height.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the space character.
 * 
 * The values at `array[height]` and `array[height][width]` will be set 
 * to 0 for null termination. It is advised that you create a `char**` 
 * with this method and pass it to other functions presented in this 
 * header file, rather than create a `char**` and pass it to other 
 * functions, in order to avoid undefined behavior.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @return The 2D Character Array.
 */
char** CharBuilder_create(int width, int height);


/**
 * @brief Creates a 2D Character Array with a specific width and height, filled with a specific character.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the specified character.
 * 
 * The values at `array[height]` and `array[height][width]` will be set 
 * to 0 for null termination. It is advised that you create a `char**` 
 * with this method and pass it to other functions presented in this 
 * header file, rather than create a `char**` and pass it to other 
 * functions, in order to avoid undefined behavior.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @param c The character to fill the array with.
 * @return The 2D Character Array.
 */
char** CharBuilder_createFilled(int width, int height, char c);

/**
 * @brief Sets a character in a 2D Character Array.
 * 
 * This method sets a character at the specified x and y
 * coordinates. If the x or y coordinate is out of bounds, the method will return -1.
 * 
 * @param array The 2D Character Array.
 * @param x The x coordinate.
 * @param y The y coordinate
 * @param c The character to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_setChar(char** array, int x, int y, char c);

/**
 * @brief Draws a horizontal line in a 2D Character Array.
 * 
 * This method draws a horizontal line in a 2D Character Array. If the x or y coordinate
 * is out of bounds, the method will return -1. If the line goes out of bounds, the line
 * will be cropped.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the line.
 * @param c The character to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_hLine(char** array, int x, int y, int width, char c);

/**
 * @brief Draws a vertical line in a 2D Character Array.
 * 
 * This method draws a vertical line in a 2D Character Array. If the x or y coordinate
 * is out of bounds, the method will return -1. If the line goes out of bounds, the line
 * will be cropped.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param height The height of the line.
 * @param c The character to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_vLine(char** array, int x, int y, int height, char c);

/**
 * @brief Draws a rectangle in a 2D Character Array.
 * 
 * This method draws a rectangle in a 2D Character Array. If the x or y coordinate
 * is out of bounds, the method will return -1. If the rectangle goes out of bounds,
 * the rectangle will be cropped.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The character to draw the rectangle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_rect(char** array, int x, int y, int width, int height, char c);

/**
 * @brief Fills a rectangle in a 2D Character Array.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The character to fill the rectangle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_fillRect(char** array, int x, int y, int width, int height, char c);

/**
 * @brief Fills the entire 2D Character Array.
 * @param array The character array.
 * @param c The character to fill the array with.
 * @return 0 if successful, -1 if an error occured.
 */
int CharBuilder_fill(char** array, char c);

/**
 * @brief Draws a circle in a 2D Character Array.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The character to draw the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_circle(char** array, int x, int y, int radius, char c);

/**
 * @brief Fills a circle in a 2D Character Array.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The character to fill the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_fillCircle(char** array, int x, int y, int radius, char c);

/**
 * @brief Draws an ellipse in a 2D Character Array.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param xradius The x radius of the ellipse.
 * @param yradius The y radius of the ellipse.
 * @param c The character to draw the ellipse with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_ellipse(char** array, int x, int y, int xradius, int yradius, char c);

/**
 * @brief Fills an ellipse in a 2D Character Array.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param xradius The x radius of the ellipse.
 * @param yradius The y radius of the ellipse.
 * @param c The character to fill the ellipse with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_fillEllipse(char** array, int x, int y, int xradius, int yradius, char c);

/**
 * @brief Draws a line in a 2D Character Array.
 * @param array The character array.
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 * @param c The character to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_line(char** array, int x1, int y1, int x2, int y2, char c);

/**
 * @brief Draws a polygon in a 2D Character Array.
 * 
 * This method draws a polygon in a 2D Character Array. If the x or y coordinate
 * is out of bounds, the method will return -1. If the polygon goes out of bounds,
 * the polygon will be cropped.
 * 
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param sides The number of sides of the polygon.
 * @param radius The radius of the polygon.
 * @param c The character to draw the polygon with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_polygon(char** array, int x, int y, int sides, int radius, char c);

/**
 * @brief Fills a polygon in a 2D Character Array.
 * 
 * This method fills a polygon in a 2D Character Array. If the x or y coordinate
 * is out of bounds, the method will return -1. If the polygon goes out of bounds,
 * the polygon will be cropped.
 * 
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param sides The number of sides of the polygon.
 * @param radius The radius of the polygon.
 * @param c The character to fill the polygon with.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_fillPolygon(char** array, int x, int y, int sides, int radius, char c);

/**
 * @brief Writes text in a 2D Character Array.
 * 
 * If the text is too long to fit in the array, it will be wrapped to
 * the next line. If the text is too long to fit in the array at all,
 * the string will be truncated.
 * 
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param text The text to draw.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_text(char** array, int x, int y, char* text);

// Utility Functions - Sizing

/**
 * @brief Resizes a 2D Character Array to a specific width and height.
 * 
 * This method resizes a 2D Character Array to a specific width and height. If the new
 * width or height is smaller than the original width or height, the 2D Character Array
 * will be cropped. If the new width or height is larger than the original width or height,
 * the 2D Character Array will be padded with empty space.
 * 
 * @param array The 2D Character Array.
 * @param width The new width.
 * @param height The new height.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_resize(char** array, int width, int height);

/**
 * @brief Resizes a 2D Character Array to a specific width and height with padding.
 * 
 * This method resizes a 2D Character Array to a specific width and height with padding.
 * If the new width or height is smaller than the original width or height, the ASCII art
 * character will be cropped. If the new width or height is larger than the original width
 * or height, the 2D Character Array will be padded with the specified character.
 * 
 * @param array The 2D Character Array.
 * @param width The new width.
 * @param height The new height.
 * @param padding The padding character.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_resizeWithPadding(char** array, int width, int height, char padding);

/**
 * @brief Centers a 2D Character Array.
 * 
 * This method centers a 2D Character Array both horizontally and vertically. It will
 * reposition the characters depending on whether there is whitespace surrounding other
 * characters.
 * 
 * @param array The 2D Character Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_center(char** array);

/**
 * @brief Resizes and centers a 2D Character Array.
 * 
 * This method resizes and centers a 2D Character Array to a specific width and height.
 * If the new width or height is smaller than the original width or height, the ASCII art
 * character will be cropped. If the new width or height is larger than the original width
 * or height, the 2D Character Array will be padded with the specified character.
 * 
 * @param array The 2D Character Array.
 * @param width The new width.
 * @param height The new height.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_resizeAndCenter(char** array, int width, int height);

// Utility Functions - Transformation

/**
 * @brief Rotates a 2D Character Array.
 * 
 * This method rotates a 2D Character Array by a specific number of radians. The
 * rotation will be done around the center of the 2D Character Array.
 * 
 * @param array The 2D Character Array.
 * @param radians The number of radians to rotate.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_rotate(char** array, double radians);

/**
 * @brief Gets the rotation angle of a 2D Character Array.
 * 
 * This method returns the rotation angle of a 2D Character Array in radians. The angle
 * will be between -PI and PI. If the 2D Character Array is not rotated, the method will
 * return 0. Rotation is calculated by the number of whitespace characters in the 2D
 * Character Array, meaning this method relies on whether it is visually rotated or not.
 * 
 * @param array The 2D Character Array.
 * @return The rotation angle of the 2D Character Array in radians.
 */
double CharBuilder_getRotationAngle(char** array);

/**
 * @brief Flips a 2D Character Array horizontally.
 * 
 * @param array The 2D Character Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_hFlip(char** array);

/**
 * @brief Flips a 2D Character Array vertically.
 * 
 * @param array The 2D Character Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_vFlip(char** array);

/**
 * @brief Transposes a 2D Character Array.
 * 
 * This method transposes a 2D Character Array, then
 * returns the transposed 2D Character Array. The original
 * 2D Character Array will be freed.
 * 
 * @param array The 2D Character Array.
 * @return The transposed 2D Character Array.
 */
char** CharBuilder_transpose(char** array);

/**
 * @brief Replaces all instances of a character in a 2D Character Array.
 * 
 * @param array The 2D Character Array.
 * @param find The character to find.
 * @param replace The character to replace the found character with.
 * @return The number of characters replaced, 0 if none, or -1 if an error occurred.
 */
int CharBuilder_replaceAll(char** array, char find, char replace);

/**
 * @brief Scales a 2D Character Array.
 * 
 * This method scales a 2D Character Array by a specific factor. The scaling
 * will be done around the center of the 2D Character Array.
 * 
 * The original 2D Character Array will be freed.
 * 
 * @param array The 2D Character Array.
 * @param scale The factor to scale the 2D Character Array by.
 * @return 0 if successful, -1 if an error occurred.
 */
char** CharBuilder_scale(char** array, double scale);

// Utility Functions - Gradient

/**
 * @brief Creates a gradient in a 2D Character Array.
 * 
 * This method creates a gradient in a 2D Character Array. The gradient will
 * be created from the start character to the end character. The gradient
 * will be created in the specified direction.
 * 
 * This function supports a simple gradient from one character to another
 * at 50% each.
 * 
 * @param array The 2D Character Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param start The start character.
 * @param end The end character.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_gradient(char** array, int x, int y, int width, int height, char start, char end, enum CmdFX_GradientDirection direction);

/**
 * @brief Creates a multi-gradient in a 2D Character Array.
 * 
 * This method creates a multi-gradient in a 2D Character Array. The gradient will
 * be created from the start character to the end character. The gradient will
 * be created in the specified direction.
 * 
 * This function supports a multi-gradient from multiple characters at
 * equal intervals.
 * 
 * @param array The 2D Character Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param numChars The number of characters in the gradient.
 * @param gradient The gradient characters.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_multiGradient(char** array, int x, int y, int width, int height, int numChars, char* gradient, enum CmdFX_GradientDirection direction);

/**
 * @brief Creates a multi-gradient in a 2D Character Array with percentages.
 * 
 * This method creates a multi-gradient in a 2D Character Array. The gradient will
 * be created from the start character to the end character. The gradient will
 * be created in the specified direction.
 * 
 * This function supports a multi-gradient from multiple characters at
 * specific percentages.
 * 
 * @param array The 2D Character Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param numChars The number of characters in the gradient.
 * @param gradient The gradient characters.
 * @param percentages The percentages of the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_multiGradients(char** array, int x, int y, int width, int height, int numChars, char* gradient, double* percentages, enum CmdFX_GradientDirection direction);

/**
 * @brief Creates a full gradient in a 2D Character Array.
 * 
 * This method creates a full gradient in a 2D Character Array. The gradient will
 * be created from the start character to the end character. The gradient will
 * be created in the specified direction.
 * 
 * This function supports a full gradient from one character to another.
 * 
 * @param array The 2D Character Array.
 * @param start The start character.
 * @param end The end character.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_gradientFull(char** array, char start, char end, enum CmdFX_GradientDirection direction);

/**
 * @brief Creates a multi-gradient in a 2D Character Array.
 * 
 * This method creates a multi-gradient in a 2D Character Array. The gradient will
 * be created from the start character to the end character. The gradient will
 * be created in the specified direction.
 * 
 * This function supports a multi-gradient from multiple characters at
 * equal intervals.
 * 
 * @param array The 2D Character Array.
 * @param numChars The number of characters in the gradient.
 * @param gradient The gradient characters.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_multiGradientFull(char** array, int numChars, char* gradient, enum CmdFX_GradientDirection direction);

/**
 * @brief Creates a multi-gradient in a 2D Character Array with percentages.
 * 
 * This method creates a multi-gradient in a 2D Character Array. The gradient will
 * be created from the start character to the end character. The gradient will
 * be created in the specified direction.
 * 
 * This function supports a multi-gradient from multiple characters at
 * specific percentages.
 * 
 * @param array The 2D Character Array.
 * @param numChars The number of characters in the gradient.
 * @param gradient The gradient characters.
 * @param percentages The percentages of the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int CharBuilder_multiGradientsFull(char** array, int numChars, char* gradient, double* percentages, enum CmdFX_GradientDirection direction);

#pragma endregion

#pragma region ANSI Builder

// Utility Functions - General (ANSI)

/**
 * @brief Gets the width of a 2D String Array.
 * @param array The 2D String Array.
 * @return The width of the array.
 */
int getAnsiArrayWidth(char*** array);

/**
 * @brief Gets the height of a 2D String Array.
 * @param array The 2D String Array.
 * @return The height of the array.
 */
int getAnsiArrayHeight(char*** array);

/**
 * @brief Creates a copy of a 2D String Array.
 * @param array The 2D String Array.
 * @return A copy of the 2D Array.
 */
char*** createAnsiArrayCopy(char*** array);

// Core Functions (ANSI)

/**
 * @brief Creates a 2D String Array with a specific width and height.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the space character.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @return The 2D String Array.
 */
char*** AnsiBuilder_create(int width, int height);

/**
 * @brief Creates a 2D String Array with a specific width and height, filled with a specific string.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the specified string.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @return The 2D String Array.
 */
char*** AnsiBuilder_createFilled(int width, int height, char* c);

/**
 * @brief Sets an ANSI code in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * This method sets an ANSI code at the specified x and y
 * coordinates. If the x or y coordinate is out of bounds, the method will return -1.
 * 
 * The values at `array[height]` and `array[height][width]` will be set 
 * to 0 for null termination. It is advised that you create a `char***` 
 * with this method and pass it to other functions presented in this 
 * header file, rather than create a `char***` and pass it to other 
 * functions, in order to avoid undefined behavior.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate
 * @param c The ANSI code to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_setAnsi(char*** array, int x, int y, char* c);

/**
 * @brief Draws a horizontal line in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the line.
 * @param c The ANSI code to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_hLine(char*** array, int x, int y, int width, char* c);

/**
 * @brief Draws a vertical line in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param height The height of the line.
 * @param c The ANSI to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_vLine(char*** array, int x, int y, int height, char* c);

/**
 * @brief Draws a rectangle in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The ANSI code to draw the rectangle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_rect(char*** array, int x, int y, int width, int height, char* c);

/**
 * @brief Fills a rectangle in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The ANSI code to fill the rectangle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_fillRect(char*** array, int x, int y, int width, int height, char* c);


/**
 * @brief Fills the entire 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param c The ANSI code to fill the array with.
 * @return 0 if successful, -1 if an error occured.
 */
int AnsiBuilder_fill(char*** array, char* c);

/**
 * @brief Draws a circle in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The ANSI code to draw the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_circle(char*** array, int x, int y, int radius, char* c);

/**
 * @brief Fills a circle in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The ANSI code to fill the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_fillCircle(char*** array, int x, int y, int radius, char* c);

/**
 * @brief Draws an ellipse in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param xradius The x radius of the ellipse.
 * @param yradius The y radius of the ellipse.
 * @param c The ANSI code to draw the ellipse with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_ellipse(char*** array, int x, int y, int xradius, int yradius, char* c);

/**
 * @brief Fills an ellipse in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param xradius The x radius of the ellipse.
 * @param yradius The y radius of the ellipse.
 * @param c The ANSI code to fill the ellipse with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_fillEllipse(char*** array, int x, int y, int xradius, int yradius, char* c);

/**
 * @brief Draws a line in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * @param array The string array.
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 * @param c The ANSI code to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_line(char*** array, int x1, int y1, int x2, int y2, char* c);

/**
 * @brief Draws a polygon in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * This method draws a polygon in a 2D String Array. If the x or y coordinate
 * is out of bounds, the method will return -1. If the polygon goes out of bounds,
 * the polygon will be cropped.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param sides The number of sides of the polygon.
 * @param radius The radius of the polygon.
 * @param c The ANSI code to draw the polygon with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_polygon(char*** array, int x, int y, int sides, int radius, char* c);

/**
 * @brief Fills a polygon in a 2D String Array.
 * 
 * Like all methods in AnsiBuilder, this does **not** append anything to the string.
 * You will need to input the full ANSI code, like "\033[31m" for red text.
 * 
 * This method fills a polygon in a 2D String Array. If the x or y coordinate
 * is out of bounds, the method will return -1. If the polygon goes out of bounds,
 * the polygon will be cropped.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param sides The number of sides of the polygon.
 * @param radius The radius of the polygon.
 * @param c The ANSI code to fill the polygon with.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_fillPolygon(char*** array, int x, int y, int sides, int radius, char* c);

// Utility Functions - Transformation (ANSI)

/**
 * @brief Rotates a 2D String Array.
 * 
 * This method rotates a 2D String Array by a specific number of radians. The
 * rotation will be done around the center of the 2D String Array.
 * 
 * @param array The 2D String Array.
 * @param radians The number of radians to rotate.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_rotate(char*** array, double radians);

/**
 * @brief Flips a 2D String Array horizontally.
 * 
 * @param array The 2D String Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_hFlip(char*** array);

/**
 * @brief Flips a 2D String Array vertically.
 * 
 * @param array The 2D String Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_vFlip(char*** array);

/**
 * @brief Transposes a 2D String Array.
 * 
 * This method transposes a 2D String Array, then
 * returns the transposed 2D String Array. The original
 * 2D String Array will be freed.
 * 
 * @param array The 2D String Array.
 * @return The transposed 2D String Array.
 */
char*** AnsiBuilder_transpose(char*** array);

/**
 * @brief Replaces all instances of an ANSI code in a 2D String Array.
 * 
 * @param array The 2D String Array.
 * @param find The ANSI code to find.
 * @param replace The ANSI code to replace the found ANSI code with.
 * @return The number of ANSI codes replaced, 0 if none, or -1 if an error occurred.
 */
int AnsiBuilder_replaceAll(char*** array, char* find, char* replace);

/**
 * @brief Scales a 2D String Array.
 * 
 * This method scales a 2D String Array by a specific factor. The scaling
 * will be done around the center of the 2D String Array.
 * 
 * The original 2D String Array will be freed.
 * 
 * @param array The 2D String Array.
 * @param scale The factor to scale the 2D String Array by.
 * @return 0 if successful, -1 if an error occurred.
 */
char*** AnsiBuilder_scale(char*** array, double scale);

// Utility Functions - Gradients (ANSI)

/**
 * @brief Sets a foreground gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a simple gradient with 2 colors at 50% each.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param start The start RGB code.
 * @param end The end RGB code.
 * @param direction 
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_gradientForeground(char*** array, int x, int y, int width, int height, int start, int end, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a background gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a simple gradient with 2 colors at 50% each.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param start The start RGB code.
 * @param end The end RGB code.
 * @param direction 
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_gradientBackground(char*** array, int x, int y, int width, int height, int start, int end, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a foreground gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at equal
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientForeground(char*** array, int x, int y, int width, int height, int numColors, int* colors, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a background gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at equal
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientBackground(char*** array, int x, int y, int width, int height, int numColors, int* colors, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a foreground gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at different
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param percentages The percentages of each color in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientsForeground(char*** array, int x, int y, int width, int height, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a background gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at different
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param percentages The percentages of each color in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientsBackground(char*** array, int x, int y, int width, int height, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a background gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a full gradient with multiple colors at different
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param start The start RGB code.
 * @param end The end RGB code.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_gradientForegroundFull(char*** array, int start, int end, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a background gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a full gradient with multiple colors at different
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param start The start RGB code.
 * @param end The end RGB code.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_gradientBackgroundFull(char*** array, int start, int end, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a foreground gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at equal
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientForegroundFull(char*** array, int numColors, int* colors, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a background gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at equal
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientBackgroundFull(char*** array, int numColors, int* colors, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a foreground gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at different
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param percentages The percentages of each color in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientsForegroundFull(char*** array, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

/**
 * @brief Sets a background gradient in a 2D String Array.
 * 
 * This method sets a gradient in a 2D String Array. The gradient will be
 * set from the start RGB code to the end RGB code in the specified bounds.
 * The direction of the gradient can be specified.
 * 
 * This function supports a multi-gradient with multiple colors at different
 * intervals.
 * 
 * @param array The 2D String Array.
 * @param numColors The number of colors in the gradient
 * @param colors The RGB colors in the gradient.
 * @param percentages The percentages of each color in the gradient.
 * @param direction The direction of the gradient.
 * @return 0 if successful, -1 if an error occurred.
 */
int AnsiBuilder_multiGradientsBackgroundFull(char*** array, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

#pragma endregion

#ifdef __cplusplus
}
#endif