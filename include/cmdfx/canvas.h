#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)

#define COLOR_BLACK           0x0000
#define COLOR_BLUE            0x0001
#define COLOR_GREEN           0x0002
#define COLOR_CYAN            0x0003
#define COLOR_RED             0x0004
#define COLOR_MAGENTA         0x0005
#define COLOR_BROWN           0x0006
#define COLOR_LIGHT_GRAY      0x0007
#define COLOR_DARK_GRAY       0x0008
#define COLOR_LIGHT_BLUE      0x0009 
#define COLOR_LIGHT_GREEN     0x000A
#define COLOR_LIGHT_CYAN      0x000B
#define COLOR_LIGHT_RED       0x000C
#define COLOR_LIGHT_MAGENTA   0x000D
#define COLOR_YELLOW          0x000E
#define COLOR_WHITE           0x000F

#else

#define COLOR_BLACK         0
#define COLOR_RED           1
#define COLOR_GREEN         2
#define COLOR_YELLOW        3
#define COLOR_BLUE          4
#define COLOR_MAGENTA       5
#define COLOR_CYAN          6
#define COLOR_WHITE         7
#define COLOR_BRIGHT_BLACK  8
#define COLOR_BRIGHT_RED    9
#define COLOR_BRIGHT_GREEN  10
#define COLOR_BRIGHT_YELLOW 11
#define COLOR_BRIGHT_BLUE   12
#define COLOR_BRIGHT_MAGENTA 13
#define COLOR_BRIGHT_CYAN   14
#define COLOR_BRIGHT_WHITE  15

#endif

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
 * Gets the color of the text.
 * @return The color of the text.
 */
int Canvas_getColor();

/**
 * Sets the color of the text.
 * @param color The color of the text.
 */
void Canvas_setColor(int color);

/**
 * Sets a character at a specific position.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param c The character to set.
 */
void Canvas_setChar(int x, int y, char c);

#ifdef __cplusplus
}
#endif