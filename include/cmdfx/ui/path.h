/**
 * @file path.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Draw on Canvas using SVG-like path commands
 * @version 0.2.0
 * @date 2025-04-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Moves the current point to the specified coordinates.
 * @param x The x coordinate of the point
 * @param y The y coordinate of the point
 */
#define CMDFX_PATH_COMMAND_MOVETO_ABSOLUTE 'M'
/**
 * Moves the current point to the specified coordinates relative to the current
 * point.
 * @param dx The x coordinate of the point relative to the current point
 * @param dy The y coordinate of the point relative to the current point
 */
#define CMDFX_PATH_COMMAND_MOVETO_RELATIVE 'm'
/**
 * Draws a line from the current point to the specified coordinates.
 * @param x The x coordinate of the point
 * @param y The y coordinate of the point
 */
#define CMDFX_PATH_COMMAND_LINETO_ABSOLUTE 'L'
/**
 * Draws a line from the current point to the specified coordinates relative to
 * the current point.
 * @param dx The x coordinate of the point relative to the current point
 * @param dy The y coordinate of the point relative to the current point
 */
#define CMDFX_PATH_COMMAND_LINETO_RELATIVE 'l'
/**
 * Draws a horizontal line from the current point to the specified x coordinate.
 * @param x The x coordinate of the point
 */
#define CMDFX_PATH_COMMAND_HLINETO_ABSOLUTE 'H'
/**
 * Draws a horizontal line from the current point to the specified x coordinate
 * relative to the current point.
 * @param dx The x coordinate of the point relative to the current point
 */
#define CMDFX_PATH_COMMAND_HLINETO_RELATIVE 'h'
/**
 * Draws a vertical line from the current point to the specified y coordinate.
 * @param y The y coordinate of the point
 */
#define CMDFX_PATH_COMMAND_VLINETO_ABSOLUTE 'V'
/**
 * Draws a vertical line from the current point to the specified y coordinate
 * relative to the current point.
 * @param dy The y coordinate of the point relative to the current point
 */
#define CMDFX_PATH_COMMAND_VLINETO_RELATIVE 'v'
/**
 * Draws a rectangle with the specified coordinates.
 * @param x1 The x coordinate of the top-left corner of the rectangle
 * @param y1 The y coordinate of the top-left corner of the rectangle
 * @param x2 The x coordinate of the bottom-right corner of the rectangle
 * @param y2 The y coordinate of the bottom-right corner of the rectangle
 */
#define CMDFX_PATH_COMMAND_RECT_ABSOLUTE 'R'
/**
 * Draws a rectangle with the specified coordinates relative to the current
 * point.
 * @param dx1 The x coordinate of the top-left corner of the rectangle relative
 * to the current point
 * @param dy1 The y coordinate of the top-left corner of the rectangle relative
 * to the current point
 * @param dx2 The x coordinate of the bottom-right corner of the rectangle
 * relative to the current point
 * @param dy2 The y coordinate of the bottom-right corner of the rectangle
 * relative to the current point
 */
#define CMDFX_PATH_COMMAND_RECT_RELATIVE 'r'
/**
 * Draws an ellipse with the specified coordinates.
 * @param cx The x coordinate of the center of the ellipse
 * @param cy The y coordinate of the center of the ellipse
 * @param rx The x radius of the ellipse
 * @param ry The y radius of the ellipse
 * @param x_axis_rotation The rotation of the ellipse in degrees
 */
#define CMDFX_PATH_COMMAND_ELLIPSE_ABSOLUTE 'E'
/**
 * Draws an ellipse with the specified coordinates relative to the current
 * point.
 * @param dx The x coordinate of the center of the ellipse relative to the
 * current point
 * @param dy The y coordinate of the center of the ellipse relative to the
 * current point
 * @param rx The x radius of the ellipse
 * @param ry The y radius of the ellipse
 * @param x_axis_rotation The rotation of the ellipse in degrees
 */
#define CMDFX_PATH_COMMAND_ELLIPSE_RELATIVE 'e'
/**
 * Draws a quadratic Bezier curve from the current point to the specified
 * coordinates.
 * @param x1 The x coordinate of the control point
 * @param y1 The y coordinate of the control point
 * @param x2 The x coordinate of the end point
 * @param y2 The y coordinate of the end point
 */
#define CMDFX_PATH_COMMAND_QUAD_ABSOLUTE 'Q'
/**
 * Draws a quadratic Bezier curve from the current point to the specified
 * coordinates relative to the current point.
 * @param dx1 The x coordinate of the control point relative to the current
 * point
 * @param dy1 The y coordinate of the control point relative to the current
 * point
 * @param dx2 The x coordinate of the end point relative to the current point
 * @param dy2 The y coordinate of the end point relative to the current point
 */
#define CMDFX_PATH_COMMAND_QUAD_RELATIVE 'q'
/**
 * Draws a cubic Bezier curve from the current point to the specified
 * coordinates.
 * @param x1 The x coordinate of the first control point
 * @param y1 The y coordinate of the first control point
 * @param x2 The x coordinate of the second control point
 * @param y2 The y coordinate of the second control point
 * @param x3 The x coordinate of the end point
 * @param y3 The y coordinate of the end point
 */
#define CMDFX_PATH_COMMAND_CUBIC_ABSOLUTE 'C'
/**
 * Draws a cubic Bezier curve from the current point to the specified
 * coordinates relative to the current point.
 * @param dx1 The x coordinate of the first control point relative to the
 * current point
 * @param dy1 The y coordinate of the first control point relative to the
 * current point
 * @param dx2 The x coordinate of the second control point relative to the
 * current point
 * @param dy2 The y coordinate of the second control point relative to the
 * current point
 * @param dx3 The x coordinate of the end point relative to the current point
 * @param dy3 The y coordinate of the end point relative to the current point
 */
#define CMDFX_PATH_COMMAND_CUBIC_RELATIVE 'c'
/**
 * Draws an elliptical arc from the current point to the specified coordinates.
 * @param rx The x radius of the ellipse
 * @param ry The y radius of the ellipse
 * @param x_axis_rotation The rotation of the ellipse in degrees
 * @param large_arc_flag A flag indicating whether to use the large arc
 * @param sweep_flag A flag indicating whether to use the positive angle
 * @param x2 The x coordinate of the end point
 * @param y2 The y coordinate of the end point
 */
#define CMDFX_PATH_COMMAND_ARC_ABSOLUTE 'A'
/**
 * Draws an elliptical arc from the current point to the specified coordinates
 * relative to the current point.
 * @param rx The x radius of the ellipse
 * @param ry The y radius of the ellipse
 * @param x_axis_rotation The rotation of the ellipse in degrees
 * @param large_arc_flag A flag indicating whether to use the large arc
 * @param sweep_flag A flag indicating whether to use the positive angle
 * @param dx2 The x coordinate of the end point relative to the current point
 * @param dy2 The y coordinate of the end point relative to the current point
 */
#define CMDFX_PATH_COMMAND_ARC_RELATIVE 'a'

    /**
     * @brief Draws a path on the canvas using the specified path string.
     *
     * This function interprets the path string as a series of SVG-like commands
     * and draws the corresponding shapes on the canvas. The path string can
     * contain commands for moving to a point, drawing lines, rectangles,
     * ellipses, and curves, as well as closing the path.
     *
     * The function will make no attempt to validate the path string, so it is
     * the responsibility of the caller to ensure that the path string is
     * well-formed and contains valid commands. The function will return -1 if
     * the path string is NULL or if an error occurs while drawing the path.
     *
     * @param path The path string to draw
     * @param ch The character to draw
     * @return 0 on success, -1 on error
     */
    int Canvas_path(const char* path, char ch);

#ifdef __cplusplus
}
#endif