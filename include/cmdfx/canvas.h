/**
 * @file canvas.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Canvas API for drawing shapes and text on the terminal.
 * @version 0.1.0
 * @date 2025-01-20
 * 
 * @copyright Copyright (c) 2025
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Core Functions

/**
 * Gets the width of the canvas, which is the width of the terminal.
 * @return The width of the canvas.
 */
int Canvas_getWidth();

/**
 * Gets the height of the canvas, which is the height of the terminal.
 * @return The height of the canvas.
 */
int Canvas_getHeight();

/**
 * Clears the screen.
 */
void Canvas_clearScreen();

/**
 * Sets the cursor position.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void Canvas_setCursor(int x, int y);

/**
 * Gets the cursor position.
 * @return The cursor position.
 */
int Canvas_getCursorX();

/**
 * Gets the cursor position.
 * @return The cursor position.
 */
int Canvas_getCursorY();

/**
 * Sets a character at a specific position.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param c The character to set.
 */
void Canvas_setChar(int x, int y, char c);

/**
 * @brief Sets the ANSI code at the current cursor position.
 * 
 * This method sets the ANSI code at a specific position. The ANSI code is a
 * series of semicolon-separated numbers and letters that control the terminal
 * behavior. For example, the ANSI code "\033[31m" sets the text color to red.
 * The prefix "\033[" and the suffix "m" are already provided for you.
 * 
 * @param ansi The ANSI code.
 */
void Canvas_setAnsiCurrent(const char* ansi);

/**
 * @brief Sets the ANSI code at a specific position.
 * 
 * This method sets the ANSI code at a specific position. The ANSI code is a
 * series of semicolon-separated numbers and letters that control the terminal
 * behavior. For example, the ANSI code "\033[31m" sets the text color to red.
 * The prefix "\033[" and the suffix "m" are already provided for you.
 * 
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param ansi The ANSI code.
 */
void Canvas_setAnsi(int x, int y, const char* ansi);

// Utility Functions - ANSI

/**
 * @brief Gets the cursor visibility.
 * 
 * On POSIX, this is handled by an internal variable. On Windows, this is
 * determined by the cursor visibility state. Therefore, this method is often
 * less reliable on POSIX systems, since it cannot detect changes made by other
 * programs.
 * 
 * @return 1 if the cursor is visible, 0 if it is hidden.
 */
int Canvas_isCursorVisible();

/**
 * Hides the cursor.
 */
void Canvas_hideCursor();

/**
 * Shows the cursor.
 */
void Canvas_showCursor();

/**
 * Resets all formatting at the current cursor position.
 */
void Canvas_resetFormat();

/**
 * Sets the foreground color at the current cursor position.
 * @param rgb The RGB color.
 */
void Canvas_setForeground(int rgb);

/**
 * Sets the background color at the current cursor position.
 * @param rgb The RGB color.
 */
void Canvas_setBackground(int rgb);

// Utility Functions - Shapes

/**
 * Draws a horizontal line.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width Width of the line.
 * @param c The character to draw the line with.
 */
void Canvas_hLine(int x, int y, int width, char c);

/**
 * Draws a vertical line.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param height Height of the line.
 * @param c The character to draw the line with.
 */
void Canvas_vLine(int x, int y, int height, char c);

/**
 * Draws a hollow rectangle.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param c The character to draw the rectangle with.
 */
void Canvas_rect(int x, int y, int width, int height, char c);

/**
 * Fills a rectangle with a character.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param c The character to fill the rectangle with.
 */
void Canvas_fillRect(int x, int y, int width, int height, char c);

/**
 * Draws a hollow circle.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param radius Radius of the circle.
 * @param c The character to draw the circle with.
 */
void Canvas_circle(int x, int y, int radius, char c);

/**
 * Fills a circle with a character.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param radius Radius of the circle.
 * @param c The character to fill the circle with.
 */
void Canvas_fillCircle(int x, int y, int radius, char c);

/**
 * Draws a hollow ellipse.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param xradius X radius of the ellipse.
 * @param yradius Y radius of the ellipse.
 * @param c The character to draw the ellipse with.
 */
void Canvas_ellipse(int x, int y, int xradius, int yradius, char c);

/**
 * Fills an ellipse with a character.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param xradius X radius of the ellipse.
 * @param yradius Y radius of the ellipse.
 * @param c The character to fill the ellipse with.
 */
void Canvas_fillEllipse(int x, int y, int xradius, int yradius, char c);

// Utility Functions - Text

/**
 * Draws text at the current cursor position.
 * @param text The text to draw.
 */
void Canvas_drawText(int x, int y, const char* text);

#pragma region ASCII Art

/**
 * Represents empty space in ASCII art.
 */
extern char ASCII_EMPTY[8][5];

/**
 * An 'A' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_A[8][5];

/**
 * An 'a' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_A[8][5];

/**
 * A 'B' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_B[8][5];

/**
 * A 'b' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_B[8][5];

/**
 * A 'C' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_C[8][5];

/**
 * A 'c' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_C[8][5];

/**
 * A 'D' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_D[8][5];

/**
 * A 'd' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_D[8][5];

/**
 * An 'E' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_E[8][5];

/**
 * An 'e' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_E[8][5];

/**
 * A 'F' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_F[8][5];

/**
 * A 'f' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_F[8][5];

/**
 * A 'G' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_G[8][5];

/**
 * A 'g' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_G[8][5];

/**
 * A 'H' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_H[8][5];

/**
 * A 'h' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_H[8][5];

/**
 * An 'I' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_I[8][5];

/**
 * An 'i' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_I[8][5];

/**
 * A 'J' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_J[8][5];

/**
 * A 'j' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_J[8][5];

/**
 * A 'K' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_K[8][5];

/**
 * A 'k' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_K[8][5];

/**
 * A 'L' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_L[8][5];

/**
 * A 'l' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_L[8][5];

/**
 * A 'M' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_M[8][5];

/**
 * A 'm' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_M[8][5];

/**
 * A 'N' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_N[8][5];

/**
 * A 'n' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_N[8][5];

/**
 * An 'O' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_O[8][5];

/**
 * An 'o' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_O[8][5];

/**
 * A 'P' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_P[8][5];

/**
 * A 'p' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_P[8][5];

/**
 * A 'Q' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_Q[8][5];

/**
 * A 'q' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_Q[8][5];

/**
 * A 'R' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_R[8][5];

/**
 * A 'r' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_R[8][5];

/**
 * A 'S' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_S[8][5];
/**
 * A 's' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_S[8][5];

/**
 * A 'T' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_T[8][5];

/**
 * A 't' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_T[8][5];

/**
 * An 'U' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_U[8][5];

/**
 * A 'u' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_U[8][5];

/**
 * A 'V' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_V[8][5];

/**
 * A 'v' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_V[8][5];

/**
 * A 'W' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_W[8][5];

/**
 * A 'w' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_W[8][5];

/**
 * An 'X' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_X[8][5];

/**
 * A 'x' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_X[8][5];

/**
 * A 'Y' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_Y[8][5];

/**
 * A 'y' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_Y[8][5];

/**
 * A 'Z' character in ASCII art using the '#' character.
 */
extern char ASCII_UPPER_Z[8][5];

/**
 * A 'z' character in ASCII art using the '#' character.
 */
extern char ASCII_LOWER_Z[8][5];

/**
 * A '0' character in ASCII art using the '#' character.
 */
extern char ASCII_ZERO[8][5];

/**
 * A '1' character in ASCII art using the '#' character.
 */
extern char ASCII_ONE[8][5];

/**
 * A '2' character in ASCII art using the '#' character.
 */
extern char ASCII_TWO[8][5];

/**
 * A '3' character in ASCII art using the '#' character.
 */
extern char ASCII_THREE[8][5];

/**
 * A '4' character in ASCII art using the '#' character.
 */
extern char ASCII_FOUR[8][5];

/**
 * A '5' character in ASCII art using the '#' character.
 */
extern char ASCII_FIVE[8][5];

/**
 * A '6' character in ASCII art using the '#' character.
 */
extern char ASCII_SIX[8][5];
/**
 * A '7' character in ASCII art using the '#' character.
 */
extern char ASCII_SEVEN[8][5];

/**
 * A '8' character in ASCII art using the '#' character.
 */
extern char ASCII_EIGHT[8][5];

/**
 * A '9' character in ASCII art using the '#' character.
 */
extern char ASCII_NINE[8][5];

/**
 * @brief Represents a mapping of ASCII characters to their respective ASCII art.
 * 
 * This 3D array is automatically initialized when `Canvas_drawAsciiText` is called.
 * 
 * This 3D array is used to map ASCII characters to their respective ASCII art. The first
 * dimension is the ASCII character, the second dimension is the row, and the third dimension
 * is the column. The ASCII art is represented as a 2D array of characters, and drawn using
 * the Canvas_drawAscii method.
 */
char ASCII_MAP[128][8][5];

#pragma endregion

/**
 * Draws an ASCII character at a specific position.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param ascii The ASCII character as a 2D array of characters.
 */
void Canvas_drawAscii(int x, int y, char ascii[8][5]);

/**
 * @brief Draws text at a specific position.
 * 
 * This method uses the Canvas_drawAscii method to draw ASCII characters. The
 * ASCII characters are stored in pre-defined 2D arrays that are declared in this header.
 * Characters that are not present in the header will be skipped.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param character The character to use when drawing.
 * @param text The text to draw.
 */
void Canvas_drawAsciiText(int x, int y, char character, const char* text);

#ifdef __cplusplus
}
#endif