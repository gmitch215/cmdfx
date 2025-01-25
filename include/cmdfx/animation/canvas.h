/**
 * @file canvas.h
 * @author Gregory Mitchell (me@gmitch215.dev)
 * @brief Animation Extensions for canvas.h
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

// Utility Functions - Shapes

/**
 * @brief Draws a horizontal line at the specified position over time.
 * 
 * This method draws a horizontal line at the specified position over time. The line
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * @param x The X position of the line.
 * @param y The Y position of the line.
 * @param width The width of the line.
 * @param c The character to use when drawing the line.
 * @param time The amount of time, in seconds, to draw the line.
 */
void Canvas_hLine_anim(int x, int y, int width, char c, double time);

/**
 * @brief Draws a vertical line at the specified position over time.
 * 
 * This method draws a vertical line at the specified position over time. The line
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * @param x The X position of the line.
 * @param y The Y position of the line.
 * @param height The height of the line.
 * @param c The character to use when drawing the line.
 * @param time The amount of time, in seconds, to draw the line.
 */
void Canvas_vLine_anim(int x, int y, int height, char c, double time);

/**
 * @brief Draws a hollow rectangle at the specified position over time.
 * 
 * This method draws a hollow rectangle at the specified position over time. The rectangle
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * The animation will start on the left, then draw the top, right, and bottom sides of the
 * rectangle.
 * 
 * @param x The X position of the rectangle.
 * @param y The Y position of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The character to use when drawing the rectangle.
 * @param time The amount of time, in seconds, to draw the rectangle.
 */
void Canvas_rect_anim(int x, int y, int width, int height, char c, double time);

/**
 * @brief Fills a rectangle at the specified position over time.
 * 
 * This method fills a rectangle at the specified position over time. The rectangle
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * The animation begins by filling the top row of the rectangle, then the second row, and
 * so on until the entire rectangle is filled.
 * 
 * @param x The X position of the rectangle.
 * @param y The Y position of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param c The character to use when filling the rectangle.
 * @param time The amount of time, in seconds, to fill the rectangle.
 */
void Canvas_fillRect_anim(int x, int y, int width, int height, char c, double time);

/**
 * @brief Draws a hollow circle at the specified position over time.
 * 
 * This method draws a hollow circle at the specified position over time. The circle
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * The animation will start at the top of the circle and draw the circle in a clockwise
 * direction.
 * 
 * @param x The X position of the circle.
 * @param y The Y position of the circle.
 * @param radius The radius of the circle.
 * @param c The character to use when drawing the circle.
 * @param time The amount of time, in seconds, to draw the circle.
 */
void Canvas_circle_anim(int x, int y, int radius, char c, double time);

/**
 * @brief Fills a circle at the specified position over time.
 * 
 * This method fills a circle at the specified position over time. The circle
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * The animation will start at the top of the circle and fill the circle in a clockwise
 * direction.
 * 
 * @param x The X position of the circle.
 * @param y The Y position of the circle.
 * @param radius The radius of the circle.
 * @param c The character to use when filling the circle.
 * @param time The amount of time, in seconds, to fill the circle.
 */
void Canvas_fillCircle_anim(int x, int y, int radius, char c, double time);

/**
 * @brief Draws a hollow ellipse at the specified position over time.
 * 
 * This method draws a hollow ellipse at the specified position over time. The ellipse
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * The animation will start at the top of the ellipse and draw the ellipse in a clockwise
 * direction.
 * 
 * @param x The X position of the ellipse.
 * @param y The Y position of the ellipse.
 * @param xradius The X radius of the ellipse.
 * @param yradius The Y radius of the ellipse.
 * @param c The character to use when drawing the ellipse.
 * @param time The amount of time, in seconds, to draw the ellipse.
 */
void Canvas_ellipse_anim(int x, int y, int xradius, int yradius, char c, double time);

/**
 * @brief Fills an ellipse at the specified position over time.
 * 
 * This method fills an ellipse at the specified position over time. The ellipse
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * The animation will start at the top of the ellipse and fill the ellipse in a clockwise
 * direction.
 * 
 * @param x The X position of the ellipse.
 * @param y The Y position of the ellipse.
 * @param xradius The X radius of the ellipse.
 * @param yradius The Y radius of the ellipse.
 * @param c The character to use when filling the ellipse.
 * @param time The amount of time, in seconds, to fill the ellipse.
 */
void Canvas_fillEllipse_anim(int x, int y, int xradius, int yradius, char c, double time);

/**
 * @brief Draws a line at the specified position over time.
 * 
 * This method draws a line at the specified position over time. The line
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time, using the specified character.
 * 
 * @param x0 The X position of the starting point.
 * @param y0 The Y position of the starting point.
 * @param x1 The X position of the ending point.
 * @param y1 The Y position of the ending point.
 * @param c The character to use when drawing the line.
 * @param time The amount of time, in seconds, to draw the line.
 */
void Canvas_line_anim(int x0, int y0, int x1, int y1, char c, double time);

// Utility Functions - Text

/**
 * @brief Draws text at the specified position over time.
 * 
 * This method draws text at the specified position over time. The text
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time.
 * 
 * @param x The X position of the text.
 * @param y The Y position of the text.
 * @param text The text to draw.
 * @param time The amount of time, in seconds, to draw the text.
 */
void Canvas_drawText_anim(int x, int y, const char* text, double time);

/**
 * @brief Draws an ASCII character at the specified position over time.
 * 
 * This method draws an ASCII character at the specified position over time. The character
 * is linearly interpolated from the starting position to the ending position over the
 * specified amount of time.
 * 
 * @param x The X position of the character.
 * @param y The Y position of the character.
 * @param ascii The ASCII character as a 2D array of characters.
 * @param time The amount of time, in seconds, to draw the character.
 */
void Canvas_drawAsciiText_anim(int x, int y, char character, const char* text, double time);

#ifdef __cplusplus
}
#endif