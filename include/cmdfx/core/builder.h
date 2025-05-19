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
int getCharArrayWidth(char** array);

/**
 * @brief Gets the height of a 2D Character Array.
 * @param array The 2D Character Array.
 * @return The height of the array.
 */
int getCharArrayHeight(char** array);

/**
 * @brief Creates a copy of a 2D Character Array.
 * @param array The 2D Character Array.
 * @return A copy of the array.
 */
char** createCharArrayCopy(char** array);

/**
 * @brief Checks if two 2D Character Arrays are the same size.
 * 
 * This method checks if two 2D Character Arrays are the same size. If the two
 * arrays are the same size, the method will return 1. If the two arrays are not
 * the same size, the method will return 0.
 * 
 * @param array1 The first 2D Character Array.
 * @param array2 The second 2D Character Array.
 * @return 1 if the arrays are the same size, 0 if the arrays are not the same size.
 */
int areCharArraysSameSize(char** array1, char** array2);

/**
 * @brief Compares two 2D Character Arrays.
 * 
 * This method compares two 2D Character Arrays. If the two arrays are equal,
 * the method will return 0. If the two arrays are not equal, the method will
 * return -1.
 * 
 * @param array1 The first 2D Character Array.
 * @param array2 The second 2D Character Array.
 * @return 0 if the arrays are equal, -1 if the arrays are not equal.
 */
int compareCharArrays(char** array1, char** array2);

/**
 * @brief Prints a 2D Character Array.
 * 
 * This method prints a 2D Character Array to the console.
 * 
 * @param array The 2D Character Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int printCharArray(char** array);

// Core Functions (Character)

/**
 * @brief Creates a 2D Character Array with a specific width and height.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the space character.
 * 
 * The values at `array[height]` will be set to 0 for null termination.
 * It is advised that you create a `char**` with this method and pass
 * it to other functions presented in this header file, rather than 
 * create a `char**` and pass it to other functions, in order to 
 * avoid undefined behavior.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @return The 2D Character Array.
 */
char** Char2DBuilder_create(int width, int height);

/**
 * @brief Creates a 2D Character Array with a specific width and height, filled with a specific character.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the specified character.
 * 
 * The values at `array[height]` will be set to 0 for null termination. 
 * It is advised that you create a `char**` with this method and pass 
 * it to other functions presented in this header file, rather than 
 * create a `char**` and pass it to other functions, in order to 
 * avoid undefined behavior.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @param c The character to fill the array with.
 * @return The 2D Character Array.
 */
char** Char2DBuilder_createFilled(int width, int height, char c);

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
int Char2DBuilder_setChar(char** array, int x, int y, char c);

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
int Char2DBuilder_hLine(char** array, int x, int y, int width, char c);

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
int Char2DBuilder_vLine(char** array, int x, int y, int height, char c);

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
int Char2DBuilder_rect(char** array, int x, int y, int width, int height, char c);

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
int Char2DBuilder_fillRect(char** array, int x, int y, int width, int height, char c);

/**
 * @brief Fills the entire 2D Character Array.
 * @param array The character array.
 * @param c The character to fill the array with.
 * @return 0 if successful, -1 if an error occured.
 */
int Char2DBuilder_fill(char** array, char c);

/**
 * @brief Draws a circle in a 2D Character Array.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The character to draw the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int Char2DBuilder_circle(char** array, int x, int y, int radius, char c);

/**
 * @brief Fills a circle in a 2D Character Array.
 * @param array The character array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The character to fill the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int Char2DBuilder_fillCircle(char** array, int x, int y, int radius, char c);

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
int Char2DBuilder_ellipse(char** array, int x, int y, int xradius, int yradius, char c);

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
int Char2DBuilder_fillEllipse(char** array, int x, int y, int xradius, int yradius, char c);

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
int Char2DBuilder_line(char** array, int x1, int y1, int x2, int y2, char c);

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
int Char2DBuilder_polygon(char** array, int x, int y, int sides, int radius, char c);

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
int Char2DBuilder_fillPolygon(char** array, int x, int y, int sides, int radius, char c);

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
int Char2DBuilder_text(char** array, int x, int y, char* text);

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
int Char2DBuilder_resize(char** array, int width, int height);

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
int Char2DBuilder_resizeWithPadding(char** array, int width, int height, char padding);

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
int Char2DBuilder_center(char** array);

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
int Char2DBuilder_resizeAndCenter(char** array, int width, int height);

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
int Char2DBuilder_rotate(char** array, double radians);

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
double Char2DBuilder_getRotationAngle(char** array);

/**
 * @brief Flips a 2D Character Array horizontally.
 * 
 * @param array The 2D Character Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int Char2DBuilder_hFlip(char** array);

/**
 * @brief Flips a 2D Character Array vertically.
 * 
 * @param array The 2D Character Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int Char2DBuilder_vFlip(char** array);

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
char** Char2DBuilder_transpose(char** array);

/**
 * @brief Replaces all instances of a character in a 2D Character Array.
 * 
 * @param array The 2D Character Array.
 * @param find The character to find.
 * @param replace The character to replace the found character with.
 * @return The number of characters replaced, 0 if none, or -1 if an error occurred.
 */
int Char2DBuilder_replaceAll(char** array, char find, char replace);

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
char** Char2DBuilder_scale(char** array, double scale);

// Utility Functions - Gradient

/**
 * @brief Represents a color gradient that can be applied .
 * 
 * A color gradient is a smooth trStringtion between two or more colors. 
 * The gradient can be applied to the foreground or background, and 
 * can be applied in different directions.
 */
enum CmdFX_GradientDirection {
    /**
     * @brief A horizontal gradient.
     */
    GRADIENT_HORIZONTAL,
    /**
     * @brief A horizontal gradient in reverse.
     */
    GRADIENT_HORIZONTAL_REVERSE,
    /**
     * @brief A vertical gradient.
     */
    GRADIENT_VERTICAL,
    /**
     * @brief A vertical gradient in reverse.
     */
    GRADIENT_VERTICAL_REVERSE,
    /**
     * @brief A 45-degree gradient.
     */
    GRADIENT_ANGLE_45,
    /**
     * @brief A 135-degree gradient.
     */
    GRADIENT_ANGLE_135,
    /**
     * @brief A 225-degree gradient.
     */
    GRADIENT_ANGLE_225,
    /**
     * @brief A 315-degree gradient.
     */
    GRADIENT_ANGLE_315,
    /**
     * @brief A radial gradient.
     */
    GRADIENT_RADIAL,
    /**
     * @brief A conical gradient.
     */
    GRADIENT_CONICAL
};

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
int Char2DBuilder_gradient(char** array, int x, int y, int width, int height, char start, char end, enum CmdFX_GradientDirection direction);

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
int Char2DBuilder_multiGradient(char** array, int x, int y, int width, int height, int numChars, char* gradient, enum CmdFX_GradientDirection direction);

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
int Char2DBuilder_multiGradients(char** array, int x, int y, int width, int height, int numChars, char* gradient, double* percentages, enum CmdFX_GradientDirection direction);

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
int Char2DBuilder_gradientFull(char** array, char start, char end, enum CmdFX_GradientDirection direction);

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
int Char2DBuilder_multiGradientFull(char** array, int numChars, char* gradient, enum CmdFX_GradientDirection direction);

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
int Char2DBuilder_multiGradientsFull(char** array, int numChars, char* gradient, double* percentages, enum CmdFX_GradientDirection direction);

#pragma endregion

#pragma region String Builder

// Utility Functions - General (String)

/**
 * @brief Gets the width of a 2D String Array.
 * @param array The 2D String Array.
 * @return The width of the array.
 */
int getStringArrayWidth(char*** array);

/**
 * @brief Gets the height of a 2D String Array.
 * @param array The 2D String Array.
 * @return The height of the array.
 */
int getStringArrayHeight(char*** array);

/**
 * @brief Creates a copy of a 2D String Array.
 * @param array The 2D String Array.
 * @return A copy of the 2D Array.
 */
char*** createStringArrayCopy(char*** array);

/**
 * @brief Clears a 2D String Array.
 * 
 * This method clears a 2D String Array. The array will be filled with
 * the space character.
 * 
 * @param array The 2D String Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int clearStringArray(char*** array);

/**
 * @brief Checks if two 2D String Arrays are the same size.
 * 
 * This method checks if two 2D String Arrays are the same size. If the two
 * arrays are the same size, the method will return 1. If the two arrays are not
 * the same size, the method will return 0.
 * 
 * @param array1 The first 2D String Array.
 * @param array2 The second 2D String Array.
 * @return 1 if the arrays are the same size, 0 if the arrays are not the same size.
 */
int areStringArraysSameSize(char*** array1, char*** array2);

/**
 * @brief Compares two 2D String Arrays.
 * 
 * This method compares two 2D String Arrays. If the two arrays are equal,
 * the method will return 0. If the two arrays are not equal, the method will
 * return -1.
 * 
 * @param array1 The first 2D String Array.
 * @param array2 The second 2D String Array.
 * @return 0 if the arrays are equal, -1 if the arrays are not equal.
 */
int compareStringArrays(char*** array1, char*** array2);

/**
 * @brief Prints a 2D String Array.
 * 
 * This method prints a 2D String Array. The array will be printed to the
 * console.
 * 
 * @param array The 2D String Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int printStringArray(char*** array);

// Core Functions (String)

/**
 * @brief Creates a 2D String Array with a specific width and height.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the space character.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @return The 2D String Array.
 */
char*** String2DBuilder_create(int width, int height);

/**
 * @brief Creates a 2D String Array with a specific width and height, filled with a specific string.
 * 
 * This method creates an array with a specific width and height. The
 * array will be initialized with the specified string.
 * 
 * @param width The width of the array.
 * @param height The height of the array.
 * @param c The string to fill the array with.
 * @return The 2D String Array.
 */
char*** String2DBuilder_createFilled(int width, int height, char* c);

/**
 * @brief Sets a string in a 2D String Array.
 * 
 * This method sets a string at the specified x and y
 * coordinates. If the x or y coordinate is out of bounds, the method will return -1.
 * 
 * The values at `array[height]` will be set  to 0 for null termination. 
 * It is advised that you create a `char***` with this method and pass 
 * it to other functions presented in this header file, rather than 
 * create a `char***` and pass it to other functions, in order to avoid 
 * undefined behavior.
 * 
 * @param array The 2D String Array.
 * @param x The x coordinate.
 * @param y The y coordinate
 * @param c The String to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_setString(char*** array, int x, int y, char* c);

/**
 * @brief Draws a horizontal line in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the line.
 * @param c The String to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_hLine(char*** array, int x, int y, int width, char* c);

/**
 * @brief Draws a vertical line in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param height The height of the line.
 * @param c The String to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_vLine(char*** array, int x, int y, int height, char* c);

/**
 * @brief Draws a rectangle in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The String to draw the rectangle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_rect(char*** array, int x, int y, int width, int height, char* c);

/**
 * @brief Fills a rectangle in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The String to fill the rectangle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_fillRect(char*** array, int x, int y, int width, int height, char* c);


/**
 * @brief Fills the entire 2D String Array.
 * 
 * @param array The string array.
 * @param c The String to fill the array with.
 * @return 0 if successful, -1 if an error occured.
 */
int String2DBuilder_fill(char*** array, char* c);

/**
 * @brief Draws a circle in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The String to draw the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_circle(char*** array, int x, int y, int radius, char* c);

/**
 * @brief Fills a circle in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param radius The radius of the circle.
 * @param c The String to fill the circle with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_fillCircle(char*** array, int x, int y, int radius, char* c);

/**
 * @brief Draws an ellipse in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param xradius The x radius of the ellipse.
 * @param yradius The y radius of the ellipse.
 * @param c The String to draw the ellipse with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_ellipse(char*** array, int x, int y, int xradius, int yradius, char* c);

/**
 * @brief Fills an ellipse in a 2D String Array.
 * 
 * @param array The string array.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param xradius The x radius of the ellipse.
 * @param yradius The y radius of the ellipse.
 * @param c The String to fill the ellipse with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_fillEllipse(char*** array, int x, int y, int xradius, int yradius, char* c);

/**
 * @brief Draws a line in a 2D String Array.
 * 
 * @param array The string array.
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 * @param c The String to draw the line with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_line(char*** array, int x1, int y1, int x2, int y2, char* c);

/**
 * @brief Draws a polygon in a 2D String Array.
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
 * @param c The String to draw the polygon with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_polygon(char*** array, int x, int y, int sides, int radius, char* c);

/**
 * @brief Fills a polygon in a 2D String Array.
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
 * @param c The String to fill the polygon with.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_fillPolygon(char*** array, int x, int y, int sides, int radius, char* c);

// Utility Functions - Transformation (String)

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
int String2DBuilder_rotate(char*** array, double radians);

/**
 * @brief Flips a 2D String Array horizontally.
 * 
 * @param array The 2D String Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_hFlip(char*** array);

/**
 * @brief Flips a 2D String Array vertically.
 * 
 * @param array The 2D String Array.
 * @return 0 if successful, -1 if an error occurred.
 */
int String2DBuilder_vFlip(char*** array);

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
char*** String2DBuilder_transpose(char*** array);

/**
 * @brief Replaces all instances of an String in a 2D String Array.
 * 
 * @param array The 2D String Array.
 * @param find The String to find.
 * @param replace The String to replace the found String with.
 * @return The number of Strings replaced, 0 if none, or -1 if an error occurred.
 */
int String2DBuilder_replaceAll(char*** array, char* find, char* replace);

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
char*** String2DBuilder_scale(char*** array, double scale);

// Utility Functions - Gradients (String)

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
int String2DBuilder_gradientForeground(char*** array, int x, int y, int width, int height, int start, int end, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_gradientBackground(char*** array, int x, int y, int width, int height, int start, int end, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientForeground(char*** array, int x, int y, int width, int height, int numColors, int* colors, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientBackground(char*** array, int x, int y, int width, int height, int numColors, int* colors, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientsForeground(char*** array, int x, int y, int width, int height, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientsBackground(char*** array, int x, int y, int width, int height, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_gradientForegroundFull(char*** array, int start, int end, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_gradientBackgroundFull(char*** array, int start, int end, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientForegroundFull(char*** array, int numColors, int* colors, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientBackgroundFull(char*** array, int numColors, int* colors, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientsForegroundFull(char*** array, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

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
int String2DBuilder_multiGradientsBackgroundFull(char*** array, int numColors, int* colors, double* percentages, enum CmdFX_GradientDirection direction);

#pragma endregion

#ifdef __cplusplus
}
#endif