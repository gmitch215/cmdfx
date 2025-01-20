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

#ifdef __cplusplus
}
#endif