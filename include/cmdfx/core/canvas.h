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
extern "C"
{
#endif

    // Core Functions

    /**
     * @brief Gets the width of the canvas, which is the width of the terminal.
     * @return The width of the canvas.
     */
    int Canvas_getWidth();

    /**
     * @brief Gets the height of the canvas, which is the height of the
     * terminal.
     * @return The height of the canvas.
     */
    int Canvas_getHeight();

    /**
     * @brief Clears the screen.
     *
     * This method uses a `system` call to clear the screen.
     */
    void Canvas_clearScreen();

    /**
     * @brief Sets the cursor position.
     * @param x X coordinate.
     * @param y Y coordinate.
     */
    void Canvas_setCursor(int x, int y);

    /**
     * @brief Gets the cursor position.
     *
     * On POSIX, this method uses the `ESC[6n` ANSI code to get the cursor
     * position. This can make the method more expensive than on Windows, where
     * the cursor position is stored internally. This method is therefore not
     * thread safe on POSIX platforms.
     *
     * On Windows, this method uses the `GetConsoleScreenBufferInfo` function to
     * get the cursor position.
     *
     * @return The cursor position.
     */
    int Canvas_getCursorX();

    /**
     * @brief Gets the cursor position.
     *
     * On POSIX, this method uses the `ESC[6n` ANSI code to get the cursor
     * position. This can make the method more expensive than on Windows, where
     * the cursor position is stored internally. This method is therefore not
     * thread safe on POSIX platforms.
     *
     * On Windows, this method uses the `GetConsoleScreenBufferInfo` function to
     * get the cursor position.
     * @return The cursor position.
     */
    int Canvas_getCursorY();

    /**
     * @brief Sets a character at a specific position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param c The character to set.
     */
    void Canvas_setChar(int x, int y, char c);

    /**
     * @brief Appends the ANSI code at the current cursor position.
     *
     * This method sets the ANSI code at a specific position. The ANSI code is a
     * series of semicolon-separated numbers and letters that control the
     * terminal behavior. For example, the ANSI code "\033[31m" sets the text
     * color to red.
     *
     * @param ansi The ANSI code.
     */
    void Canvas_setAnsiCurrent(const char* ansi);

    /**
     * @brief Appends the ANSI code at a specific position.
     *
     * This method sets the ANSI code at a specific position. The ANSI code is a
     * series of semicolon-separated numbers and letters that control the
     * terminal behavior. For example, the ANSI code "\033[31m" sets the text
     * color to red.
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
     * determined by the cursor visibility state. Therefore, this method is
     * often less reliable on POSIX systems, since it cannot detect changes made
     * by other programs.
     *
     * @return 1 if the cursor is visible, 0 if it is hidden.
     */
    int Canvas_isCursorVisible();

    /**
     * @brief Hides the cursor.
     *
     * On POSIX, this is a simple print of the ANSI code "\033[?25l", which
     * hides the cursor. On Windows, this is a call to the
     * `SetConsoleCursorInfo` function.
     */
    void Canvas_hideCursor();

    /**
     * @brief Shows the cursor.
     *
     * On POSIX, this is a simple print of the ANSI code "\033[?25h", which
     * shows the cursor. On Windows, this is a call to the
     * `SetConsoleCursorInfo` function.
     */
    void Canvas_showCursor();

    /**
     * @brief Resets all formatting at the current cursor position.
     *
     * This is a simple print of the ANSI code "\033[0m", which resets all
     * formatting at the current cursor position.
     */
    void Canvas_resetFormat();

    /**
     * @brief Sets the foreground color at the current cursor position.
     *
     * This uses the ANSI code "\033[38;2;r;g;bm", where r, g, and b are the
     * red, green, and blue values of the color, respectively.
     * @param rgb The RGB color.
     */
    void Canvas_setForeground(int rgb);

    /**
     * @brief Sets the background color at the current cursor position.
     *
     * This uses the ANSI code "\033[48;2;r;g;bm", where r, g, and b are the
     * red, green, and blue values of the color, respectively.
     * @param rgb The RGB color.
     */
    void Canvas_setBackground(int rgb);

    /**
     * @brief Sets the color at the current cursor position using the built-in
     * 8-bit color palette.
     *
     * This uses the ANSI code "\033[{ID}m", where m is the color number.
     * @param color The color number.
     */
    void Canvas_setColor8(int color);

    /**
     * @brief Sets the foreground color at the current cursor position using the
     * built-in 256-color palette.
     *
     * This uses the ANSI code "\033[38;5;{ID}m", where {ID} is the color
     * number.
     * @param color The color number.
     */
    void Canvas_setForeground256(int color);

    /**
     * @brief Sets the background color at the current cursor position using the
     * built-in 256-color palette.
     *
     * This uses the ANSI code "\033[48;5;{ID}m", where {ID} is the color
     * number.
     * @param color The color number.
     */
    void Canvas_setBackground256(int color);

    /**
     * @brief Enables the bold text attribute.
     *
     * This is a simple print of the ANSI code "\033[1m".
     */
    void Canvas_enableBold();

    /**
     * @brief Disables the bold text attribute.
     *
     * This is a simple print of the ANSI code "\033[22m".
     *
     * Both this and `Canvas_disableDim` share the same code, meaning it will
     * disable both bold and dim text if both are enabled.
     */
    void Canvas_disableBold();

    /**
     * @brief Enables the dim text attribute.
     *
     * This is a simple print of the ANSI code "\033[2m".
     *
     * Faint is not widely supported, and is often rendered as normal text.
     */
    void Canvas_enableDim();

    /**
     * @brief Disables the dim text attribute.
     *
     * This is a simple print of the ANSI code "\033[22m".
     *
     * Faint is not widely supported, and is often rendered as normal text.
     *
     * Both this and `Canvas_disableBold` share the same code, meaning it will
     * disable both bold and dim text if both are enabled.
     */
    void Canvas_disableDim();

    /**
     * @brief Enables the italic text attribute.
     *
     * This is a simple print of the ANSI code "\033[3m".
     *
     * Italic is not widely supported, and is often rendered as normal text.
     */
    void Canvas_enableItalic();

    /**
     * @brief Disables the italic text attribute.
     *
     * This is a simple print of the ANSI code "\033[23m".
     *
     * Italic is not widely supported, and is often rendered as normal text.
     */
    void Canvas_disableItalic();

    /**
     * @brief Enables the underline text attribute.
     *
     * This is a simple print of the ANSI code "\033[4m".
     */
    void Canvas_enableUnderline();

    /**
     * @brief Disables the underline text attribute.
     *
     * This is a simple print of the ANSI code "\033[24m".
     */
    void Canvas_disableUnderline();

    /**
     * @brief Enables the blink text attribute.
     *
     * This is a simple print of the ANSI code "\033[5m".
     *
     * "Blinking" refers to the text blinking on and off.
     */
    void Canvas_enableBlink();

    /**
     * @brief Disables the blink text attribute.
     *
     * This is a simple print of the ANSI code "\033[25m".
     */
    void Canvas_disableBlink();

    /**
     * @brief Enables the invert text attribute.
     *
     * This is a simple print of the ANSI code "\033[7m".
     *
     * "Invert" refers to the text and background colors being swapped.
     */
    void Canvas_enableInvert();

    /**
     * @brief Disables the invert text attribute.
     *
     * This is a simple print of the ANSI code "\033[27m".
     */
    void Canvas_disableInvert();

    /**
     * @brief Enables the hidden text attribute.
     *
     * Hidden is not widely supported, and is often rendered as normal text.
     *
     * This is a simple print of the ANSI code "\033[8m".
     *
     * "Hidden" refers to the text being hidden.
     */
    void Canvas_enableHidden();

    /**
     * @brief Disables the hidden text attribute.
     *
     * This is a simple print of the ANSI code "\033[28m".
     */
    void Canvas_disableHidden();

    /**
     * @brief Enables the strikethrough text attribute.
     *
     * Strikethrough is not widely supported, and is often rendered as normal
     * text.
     *
     * This is a simple print of the ANSI code "\033[9m".
     *
     * "Strikethrough" refers to the text having a line through the middle.
     */
    void Canvas_enableStrikethrough();

    /**
     * @brief Disables the strikethrough text attribute.
     *
     * This is a simple print of the ANSI code "\033[29m".
     */
    void Canvas_disableStrikethrough();

    // Utility Functions - Shapes

    /**
     * @brief Draws a horizontal line.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param width Width of the line.
     * @param c The character to draw the line with.
     */
    void Canvas_hLine(int x, int y, int width, char c);

    /**
     * @brief Draws a vertical line.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param height Height of the line.
     * @param c The character to draw the line with.
     */
    void Canvas_vLine(int x, int y, int height, char c);

    /**
     * @brief Draws a hollow rectangle.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param width Width of the rectangle.
     * @param height Height of the rectangle.
     * @param c The character to draw the rectangle with.
     */
    void Canvas_rect(int x, int y, int width, int height, char c);

    /**
     * @brief Fills a rectangle with a character.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param width Width of the rectangle.
     * @param height Height of the rectangle.
     * @param c The character to fill the rectangle with.
     */
    void Canvas_fillRect(int x, int y, int width, int height, char c);

    /**
     * @brief Draws a hollow circle.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param radius Radius of the circle.
     * @param c The character to draw the circle with.
     */
    void Canvas_circle(int x, int y, int radius, char c);

    /**
     * @brief Fills a circle with a character.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param radius Radius of the circle.
     * @param c The character to fill the circle with.
     */
    void Canvas_fillCircle(int x, int y, int radius, char c);

    /**
     * @brief Draws a hollow ellipse.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param xradius X radius of the ellipse.
     * @param yradius Y radius of the ellipse.
     * @param c The character to draw the ellipse with.
     */
    void Canvas_ellipse(int x, int y, int xradius, int yradius, char c);

    /**
     * @brief Fills an ellipse with a character.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param xradius X radius of the ellipse.
     * @param yradius Y radius of the ellipse.
     * @param c The character to fill the ellipse with.
     */
    void Canvas_fillEllipse(int x, int y, int xradius, int yradius, char c);

    /**
     * @brief Draws a line between two points.
     * @param x1 The x coordinate of the first point.
     * @param y1 The y coordinate of the first point.
     * @param x2 The x coordinate of the second point.
     * @param y2 The y coordinate of the second point.
     * @param c The character to draw the line with.
     */
    void Canvas_line(int x1, int y1, int x2, int y2, char c);

    /**
     * @brief Draws a polygon.
     *
     * This method draws a polygon with the specified number of sides and
     * radius. The X and Y coordinates are the center of the polygon.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param sides Number of sides.
     * @param radius Radius of the polygon.
     * @param c The character to draw the polygon with.
     */
    void Canvas_polygon(int x, int y, int sides, int radius, char c);

    /**
     * @brief Fills a polygon with a character.
     *
     * This method fills a polygon with the specified number of sides and
     * radius. The X and Y coordinates are the center of the polygon.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param sides Number of sides.
     * @param radius Radius of the polygon.
     * @param c The character to fill the polygon with.
     */
    void Canvas_fillPolygon(int x, int y, int sides, int radius, char c);

    /**
     * @brief Draws a quadratic Bezier curve.
     *
     * This method draws a quadratic Bezier curve from the current position to
     * the specified coordinates. The X and Y coordinates are the control point
     * of the curve.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param x1 The x coordinate of the control point.
     * @param y1 The y coordinate of the control point.
     * @param x2 The x coordinate of the end point.
     * @param y2 The y coordinate of the end point.
     * @param c The character to draw the curve with.
     */
    void Canvas_quad(int x, int y, int x1, int y1, int x2, int y2, char c);

    /**
     * @brief Draws a cubic Bezier curve.
     *
     * This method draws a cubic Bezier curve from the current position to the
     * specified coordinates. The X and Y coordinates are the control points of
     * the curve.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param x1 The x coordinate of the first control point.
     * @param y1 The y coordinate of the first control point.
     * @param x2 The x coordinate of the second control point.
     * @param y2 The y coordinate of the second control point.
     * @param x3 The x coordinate of the end point.
     * @param y3 The y coordinate of the end point.
     * @param c The character to draw the curve with.
     */
    void Canvas_cubic(
        int x, int y, int x1, int y1, int x2, int y2, int x3, int y3, char c
    );

    /**
     * @brief Draws an elliptical arc.
     *
     * This method draws an elliptical arc from the current position to the
     * specified coordinates. The X and Y coordinates are the center of the
     * ellipse.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param rx The x radius of the ellipse.
     * @param ry The y radius of the ellipse.
     * @param xrot The rotation of the ellipse in degrees.
     * @param arcflag 1 if the arc is greater than 180 degrees, 0 otherwise.
     * @param sweepflag 1 if the arc is drawn in a positive-angle direction, 0
     * otherwise.
     * @param dx The x coordinate of the end point.
     * @param dy The y coordinate of the end point.
     * @param c The character to draw the arc with.
     */
    void Canvas_arc(
        int x, int y, int rx, int ry, double xrot, int arcflag, int sweepflag,
        int dx, int dy, char c
    );

    // Utility Functions - Text

    /**
     * @brief Draws text at the current cursor position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param text The text to draw.
     */
    void Canvas_drawText(int x, int y, char* text);

#pragma region ASCII Art

    /**
     * @brief Represents empty space in ASCII art.
     */
    extern char ASCII_EMPTY[8][5];

    /**
     * @brief An 'A' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_A[8][5];

    /**
     * @brief An 'a' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_A[8][5];

    /**
     * @brief A 'B' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_B[8][5];

    /**
     * @brief A 'b' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_B[8][5];

    /**
     * @brief A 'C' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_C[8][5];

    /**
     * @brief A 'c' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_C[8][5];

    /**
     * @brief A 'D' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_D[8][5];

    /**
     * @brief A 'd' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_D[8][5];

    /**
     * @brief An 'E' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_E[8][5];

    /**
     * @brief An 'e' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_E[8][5];

    /**
     * @brief A 'F' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_F[8][5];

    /**
     * @brief A 'f' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_F[8][5];

    /**
     * @brief A 'G' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_G[8][5];

    /**
     * @brief A 'g' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_G[8][5];

    /**
     * @brief A 'H' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_H[8][5];

    /**
     * @brief A 'h' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_H[8][5];

    /**
     * @brief An 'I' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_I[8][5];

    /**
     * @brief An 'i' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_I[8][5];

    /**
     * @brief A 'J' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_J[8][5];

    /**
     * @brief A 'j' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_J[8][5];

    /**
     * @brief A 'K' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_K[8][5];

    /**
     * @brief A 'k' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_K[8][5];

    /**
     * @brief A 'L' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_L[8][5];

    /**
     * @brief A 'l' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_L[8][5];

    /**
     * @brief A 'M' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_M[8][5];

    /**
     * @brief A 'm' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_M[8][5];

    /**
     * @brief A 'N' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_N[8][5];

    /**
     * @brief A 'n' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_N[8][5];

    /**
     * @brief An 'O' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_O[8][5];

    /**
     * @brief An 'o' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_O[8][5];

    /**
     * @brief A 'P' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_P[8][5];

    /**
     * @brief A 'p' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_P[8][5];

    /**
     * @brief A 'Q' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_Q[8][5];

    /**
     * @brief A 'q' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_Q[8][5];

    /**
     * @brief A 'R' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_R[8][5];

    /**
     * @brief A 'r' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_R[8][5];

    /**
     * @brief A 'S' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_S[8][5];

    /**
     * @brief A 's' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_S[8][5];

    /**
     * @brief A 'T' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_T[8][5];

    /**
     * @brief A 't' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_T[8][5];

    /**
     * @brief An 'U' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_U[8][5];

    /**
     * @brief A 'u' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_U[8][5];

    /**
     * @brief A 'V' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_V[8][5];

    /**
     * @brief A 'v' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_V[8][5];

    /**
     * @brief A 'W' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_W[8][5];

    /**
     * @brief A 'w' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_W[8][5];

    /**
     * @brief An 'X' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_X[8][5];

    /**
     * @brief A 'x' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_X[8][5];

    /**
     * @brief A 'Y' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_Y[8][5];

    /**
     * @brief A 'y' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_Y[8][5];

    /**
     * @brief A 'Z' character in ASCII art using the '#' character.
     */
    extern char ASCII_UPPER_Z[8][5];

    /**
     * @brief A 'z' character in ASCII art using the '#' character.
     */
    extern char ASCII_LOWER_Z[8][5];

    /**
     * @brief A '0' character in ASCII art using the '#' character.
     */
    extern char ASCII_ZERO[8][5];

    /**
     * @brief A '1' character in ASCII art using the '#' character.
     */
    extern char ASCII_ONE[8][5];

    /**
     * @brief A '2' character in ASCII art using the '#' character.
     */
    extern char ASCII_TWO[8][5];

    /**
     * @brief A '3' character in ASCII art using the '#' character.
     */
    extern char ASCII_THREE[8][5];

    /**
     * @brief A '4' character in ASCII art using the '#' character.
     */
    extern char ASCII_FOUR[8][5];

    /**
     * @brief A '5' character in ASCII art using the '#' character.
     */
    extern char ASCII_FIVE[8][5];

    /**
     * @brief A '6' character in ASCII art using the '#' character.
     */
    extern char ASCII_SIX[8][5];

    /**
     * @brief A '7' character in ASCII art using the '#' character.
     */
    extern char ASCII_SEVEN[8][5];

    /**
     * @brief A '8' character in ASCII art using the '#' character.
     */
    extern char ASCII_EIGHT[8][5];

    /**
     * @brief A '9' character in ASCII art using the '#' character.
     */
    extern char ASCII_NINE[8][5];

    /**
     * @brief Represents a mapping of ASCII characters to their respective ASCII
     * art.
     *
     * This 3D array is automatically initialized when `Canvas_drawAsciiText` is
     * called.
     *
     * This 3D array is used to map ASCII characters to their respective ASCII
     * art. The first dimension is the ASCII character, the second dimension is
     * the row, and the third dimension is the column. The ASCII art is
     * represented as a 2D array of characters, and drawn using the
     * Canvas_drawAscii method.
     */
    extern char ASCII_MAP[128][8][5];

#pragma endregion

    /**
     * @brief Draws an ASCII character at a specific position.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param ascii The ASCII character as a 2D array of characters.
     */
    void Canvas_drawAscii(int x, int y, char ascii[8][5]);

    /**
     * @brief Draws text at a specific position.
     *
     * This method uses the Canvas_drawAscii method to draw ASCII characters.
     * The ASCII characters are stored in pre-defined 2D arrays that are
     * declared in this header. Characters that are not present in the header
     * will be skipped.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param character The character to use when drawing.
     * @param text The text to draw.
     */
    void Canvas_drawAsciiText(int x, int y, char character, const char* text);

#ifdef __cplusplus
}
#endif