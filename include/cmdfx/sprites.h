/**
 * @file sprites.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Sprites API for CmdFX.
 * @version 0.1.0
 * @date 2025-01-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Core Functions

/**
 * @brief Represents a sprite that can be drawn to the terminal.
 * 
 * A sprite is a 2D image that can be drawn to the terminal. It consists of a
 * 2D array of characters that represent the sprite, as well as a Z-index that
 * determines the order in which sprites are drawn. Sprites are useful for keeping
 * track of complex images or animations that need to be drawn to the terminal.
 */
typedef struct CmdFX_Sprite {
    /**
     * @brief The X position of the sprite.
     */
    int x;
    /**
     * @brief The Y position of the sprite.
     */
    int y;
    /**
     * @brief The width of the sprite.
     */
    int width;
    /**
     * @brief The height of the sprite.
     */
    int height;
    /**
     * @brief The text data for the sprite.
     * 
     * This is a 2D array of characters that represent the sprite. Each row
     * should be the same length, and the number of rows should be equal to the
     * height of the sprite. The sprite will be drawn starting at the X and Y
     * position, with the top-left corner of the sprite at the X and Y position.
     * The number of columns should be equal to the width of the sprite.
     */
    char** data;

    /**
     * @brief The ANSI data for the sprite.
     * 
     * This is a 2D array of strings that represent the ANSI codes for the sprite.
     * Each row should be the same length, and the number of rows should be equal 
     * to the height of the sprite. The sprite will be drawn starting at the X and 
     * Y position, with the top-left corner of the sprite at the X and Y position. 
     * The number of columns should be equal to the width of the sprite.
     */
    char*** ansi;
    /**
     * @brief The Z-index of the sprite.
     * 
     * This value determines the order in which sprites are drawn. Sprites with
     * a higher Z-index will be drawn on top of sprites with a lower Z-index. If
     * two sprites have the same Z-index, the order in which they are drawn is
     * undefined.
     */
    int z;
    /**
     * @brief The unique ID of the sprite.
     * 
     * This is assigned once the sprite is drawn on the screen. It is used to
     * identify the sprite when editing or removing it. If the sprite is not
     * drawn, this value will be 0.
     */
    int id;
} CmdFX_Sprite;

/**
 * @brief Gets the sprites that have been drawn to the terminal.
 * 
 * This method returns an array of pointers to the sprites that have been drawn
 * to the terminal. The array is terminated with a `NULL` pointer. The sprites
 * should not be modified or freed by the caller.
 * @return An array of pointers to the drawn sprites.
 */
CmdFX_Sprite** Canvas_getDrawnSprites();

/**
 * @brief Creates a new sprite.
 * 
 * This method allocates memory for a new sprite and initializes it with the
 * given parameters. The sprite should be freed with `Sprite_free` when it is no longer
 * needed.
 * 
 * You can also set `text` and `ansi` to NULL and built them later using `Sprite_setChar`, `Sprite_setAnsi`, and `Sprite_resize`.
 * 
 * This method does not draw the sprite to the terminal. To draw the sprite, use
 * the `Sprite_draw` method.
 * @param text The text data for the sprite.
 * @param ansi The ANSI data for the sprite.
 * @param z The Z-index of the sprite.
 * @return A pointer to the new sprite, or 0 if an error occurred.
 */
CmdFX_Sprite* Sprite_create(char** text, char*** ansi, int z);

/**
 * @brief Frees the memory allocated for a sprite. 
 * 
 * If the sprite is currently drawn, it will be removed.
 * @param sprite The sprite to free.
 */
void Sprite_free(CmdFX_Sprite* sprite);

/**
 * @brief Draws a sprite to the terminal.
 * 
 * This method draws a sprite to the terminal at the given position. The sprite
 * will be drawn starting at the X and Y position, with the top-left corner of
 * the sprite at the X and Y position. The sprite will be drawn on top of any
 * existing content at that position, according to its Z-index. If it is already
 * drawn, it will be redrawn at the new position.
 * 
 * If `data` is NULL, the sprite will not be drawn.
 * @param x The X position of the sprite.
 * @param y The Y position of the sprite.
 * @param sprite The sprite to draw.
 * @return 1 if the sprite was drawn, 0 if not or an error occurred.
 */
int Sprite_draw(int x, int y, CmdFX_Sprite* sprite);

/**
 * @brief Removes a sprite from the terminal, making it no longer visble.
 * @param sprite The sprite to remove.
 */
void Sprite_remove(CmdFX_Sprite* sprite);

// Utility Methods - Sprite Builder

/**
 * @brief Sets the character at the given position in the sprite.
 * 
 * This method sets the character at the given position in the sprite. The
 * position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the
 * method will return 0.
 * 
 * This method will also redraw the character if the sprite is currently drawn.
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param c The character to set.
 * @return 1 if the character was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setChar(CmdFX_Sprite* sprite, int x, int y, char c);

/**
 * @brief Fills positions in the sprite.
 * 
 * This method sets the character at a specific rectangle in the sprite. The
 * fill will start at the given position and fill the specified width and height. If
 * a position is out of bounds, it will be skipped.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * 
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param c The character to set.
 * @param width The width of the fill.
 * @param height The height of the fill.
 * @return 1 if the character was set, 0 if an error occurred.
 */
int Sprite_fillChar(CmdFX_Sprite* sprite, int x, int y, char c, int width, int height);

/**
 * @brief Fills positions in the sprite where there isn't a character.
 * 
 * This method sets the character at a specific rectangle in the sprite. The
 * fill will start at the given position and fill the specified width and height.  If
 * a position is out of bounds, it will be skipped.
 * 
 * This method will skip any positions that are already filled with a character. Existing
 * ANSI codes at empty positions will be preserved.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * 
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param c The character to set.
 * @param width The width of the fill.
 * @param height The height of the fill.
 * @return 1 if the character was set, 0 if an error occurred.
 */
int Sprite_fillCharEmpty(CmdFX_Sprite* sprite, int x, int y, char c, int width, int height);

/**
 * @brief Sets the character at all positions in the sprite.
 * 
 * This method sets the character at all positions in the sprite. The character
 * will be set for every position in the sprite, starting from the top-left
 * corner. If an error occurs, the method will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * 
 * @param sprite The sprite to modify.
 * @param c The character to set.
 * @return 1 if the character was set, 0 if an error occurred.
 */
int Sprite_fillCharAll(CmdFX_Sprite* sprite, char c);

/**
 * @brief Sets the character at all positions in the sprite where there isn't a character.
 * 
 * This method sets the character at all positions in the sprite. The character
 * will be set for every position in the sprite, starting from the top-left
 * corner. If an error occurs, the method will return 0.
 * 
 * This method will skip any positions that are already filled with a character. Existing
 * ANSI codes at empty positions will be preserved.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * 
 * @param sprite The sprite to modify.
 * @param c The character to set.
 * @return 1 if the character was set, 0 if an error occurred.
 */
int Sprite_fillCharAllEmpty(CmdFX_Sprite* sprite, char c);

/**
 * @brief Sets the ANSI code at the given position in the sprite.
 * 
 * This method sets the ANSI code at the given position in the sprite. The
 * position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the
 * method will return 0. 
 * 
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param ansi The ANSI code to set.
 * @return 1 if the ANSI code was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi);

/**
 * @brief Appends an ANSI code to the given position in the sprite.
 * 
 * This method appends the given ANSI code to the existing ANSI code at the
 * given position in the sprite. The position is specified by the X and Y
 * coordinates, where (0, 0) is the top-left corner of the sprite. If the
 * position is out of bounds, the method will return 0.
 * 
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param ansi The ANSI code to append.
 * @return 1 if the ANSI code was appended, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_appendAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi);

/**
 * @brief Fills positions in the sprite with an ANSI code.
 * 
 * This method sets the ANSI code at a specific rectangle in the sprite. The
 * fill will start at the given position and fill the specified width and height. If
 * a position is out of bounds, it will be skipped.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * 
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param ansi The ANSI code to set.
 * @param width The width of the fill.
 * @param height The height of the fill.
 * @return 1 if the ANSI code was set, 0 if an error occurred.
 */
int Sprite_fillAnsi(CmdFX_Sprite* sprite, int x, int y, char* ansi, int width, int height);

/**
 * @brief Sets the character at all positions in the sprite.
 * 
 * This method sets the character at all positions in the sprite. The character
 * will be set for every position in the sprite, starting from the top-left
 * corner. If an error occurs, the method will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * 
 * @param sprite The sprite to modify.
 * @param c The character to set.
 * @return 1 if the character was set, 0 if an error occurred.
 */
int Sprite_setAnsiAll(CmdFX_Sprite* sprite, char* ansi);

/**
 * @brief Appends an ANSI code to all positions in the sprite.
 * 
 * This method appends the given ANSI code to the existing ANSI code at all
 * positions in the sprite. The position is specified by the X and Y
 * coordinates, where (0, 0) is the top-left corner of the sprite. If the
 * position is out of bounds, the method will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * 
 * @param sprite The sprite to modify.
 * @param ansi The ANSI code to append.
 * @return 1 if the ANSI code was appended, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_appendAnsiAll(CmdFX_Sprite* sprite, char* ansi);

/**
 * @brief Creates a new sprite with the given dimensions.
 * 
 * This method creates a new sprite with the given dimensions. The sprite will
 * be filled with the specified character and ANSI code. The sprite should be
 * freed with `Sprite_free` when it is no longer needed.
 * 
 * This method does not draw the sprite to the terminal. To draw the sprite, use
 * the `Sprite_draw` method.
 * 
 * @param width The width of the sprite.
 * @param height The height of the sprite.
 * @param c The character to fill the sprite with.
 * @param ansi The ANSI code to fill the sprite with.
 * @param z The Z-index of the sprite.
 * @return A pointer to the new sprite, or 0 if an error occurred.
 */
CmdFX_Sprite* Sprite_createFilled(int width, int height, char c, char* ansi, int z);

// Utility Methods - Sizing

/**
 * @brief Resizes the sprite to the given dimensions.
 * 
 * This method is functionally identical to `Sprite_resizeWithPadding`, but
 * it will pad the sprite with spaces if it is resized larger.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to resize.
 * @param width The new width of the sprite.
 * @param height The new height of the sprite.
 * @return 1 if the sprite was resized, 0 if an error occurred.
 */
int Sprite_resize(CmdFX_Sprite* sprite, int width, int height);

/**
 * @brief Resizes the sprite to the given dimensions.
 * 
 * This method resizes the sprite to the given width and height. If the new
 * dimensions are smaller than the current dimensions, the sprite will be
 * cropped from the top left. If the new dimensions are larger than the current dimensions, the
 * sprite will be padded with the provided character. If an error occurs, the method will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to resize.
 * @param width The new width of the sprite.
 * @param height The new height of the sprite.
 * @param padding The character to pad the sprite with if it is resized larger.
 * @return 1 if the sprite was resized, 0 if an error occurred.
 */
int Sprite_resizeWithPadding(CmdFX_Sprite* sprite, int width, int height, char padding);

/**
 * @brief Centers the sprite on the terminal.
 * 
 * This method centers the sprite on the terminal, both horizontally and
 * vertically. It will reposition the `data` and `ansi` characters depending
 * on whether there is whitespace surrounding other characters.
 * 
 * The sprite will be drawn at its current position.
 * @param sprite The sprite to center.
 * @return 1 if the sprite was centered, 0 if an error occurred.
 */
int Sprite_center(CmdFX_Sprite* sprite);

/**
 * @brief Resizes and centers the sprite on the terminal.
 * 
 * This method resizes the sprite to the given width and height, and then
 * centers the sprite on the terminal, both horizontally and vertically. It
 * will reposition the `data` and `ansi` characters depending on whether there
 * is whitespace surrounding other characters.
 * 
 * The sprite will be drawn at its new position.
 * @param sprite The sprite to resize and center.
 * @param width The new width of the sprite.
 * @param height The new height of the sprite.
 * @return 1 if the sprite was resized and centered, 0 if an error occurred.
 */
int Sprite_resizeAndCenter(CmdFX_Sprite* sprite, int width, int height);

// Utility Methods - Movement

/**
 * @brief Moves a sprite to the given position.
 * @param sprite The sprite to move.
 * @param x The new X position of the sprite.
 * @param y The new Y position of the sprite.
 */
void Sprite_moveTo(CmdFX_Sprite* sprite, int x, int y);

/**
 * @brief Moves a sprite by the given amount.
 * @param sprite The sprite to move.
 * @param dx The delta X position to move the sprite by.
 * @param dy The delta Y position to move the sprite by.
 */
void Sprite_moveBy(CmdFX_Sprite* sprite, int dx, int dy);

// Utility Methods - Collisions

/**
 * @brief Gets the sprites that are colliding with the given sprite.
 * @param sprite The sprite to check for collisions.
 * @return An array of pointers to the colliding sprites.
 */
CmdFX_Sprite** Sprite_getCollidingSprites(CmdFX_Sprite* sprite);

/**
 * @brief Checks if two sprites are colliding.
 * @param sprite1 The first sprite.
 * @param sprite2 The second sprite.
 * @return 1 if the sprites are colliding, 0 otherwise.
 */
int Sprite_isColliding(CmdFX_Sprite* sprite1, CmdFX_Sprite* sprite2);

// Utility Methods - Color

/**
 * @brief Sets the foreground color of the sprite at the given position.
 * 
 * This method sets the foreground color of the sprite at the given position.
 * The position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the method
 * will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param rgb The RGB color to set.
 * @return 1 if the color was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setForeground(CmdFX_Sprite* sprite, int x, int y, int rgb);

/**
 * @brief Sets the foreground color of the sprite at the given position.
 * 
 * This method sets the foreground color of the sprite at the given position.
 * The position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the method
 * will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param rgb The RGB color to set.
 * @return 1 if the color was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setForegroundAll(CmdFX_Sprite* sprite, int rgb);

/**
 * @brief Sets the foreground color of the sprite at the given position using 256-bit color codes.
 * 
 * This method sets the foreground color of the sprite at the given position.
 * The position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the method
 * will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param color The 256-color code to set.
 * @return 1 if the color was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setForegroundAll256(CmdFX_Sprite* sprite, int color);

/**
 * @brief Sets the background color of the sprite at the given position.
 * 
 * This method sets the background color of the sprite at the given position.
 * The position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the method
 * will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param rgb The RGB color to set.
 * @return 1 if the color was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setBackground(CmdFX_Sprite* sprite, int x, int y, int rgb);

/**
 * @brief Sets the background color of the sprite at the given position.
 * 
 * This method sets the background color of the sprite at the given position.
 * The position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the method
 * will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param rgb The RGB color to set.
 * @return 1 if the color was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setBackgroundAll(CmdFX_Sprite* sprite, int rgb);

/**
 * @brief Sets the background color of the sprite at the given position using 256-bit color codes.
 * 
 * This method sets the background color of the sprite at the given position.
 * The position is specified by the X and Y coordinates, where (0, 0) is the
 * top-left corner of the sprite. If the position is out of bounds, the method
 * will return 0.
 * 
 * This method will also redraw the sprite if it is currently drawn.
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param color The 256-color code to set.
 * @return 1 if the color was set, 0 if the position is out of bounds or an error occurred.
 */
int Sprite_setBackgroundAll256(CmdFX_Sprite* sprite, int color);

// Utility Methods - Color Gradient

/**
 * @brief Represents a color gradient that can be applied to a sprite.
 * 
 * A color gradient is a smooth transition between two or more colors. The
 * gradient can be applied to the foreground or background of a sprite, and
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
     * @brief A radial gradient.
     */
    GRADIENT_RADIAL,
    /**
     * @brief A conical gradient.
     */
    GRADIENT_CONICAL
};

/**
 * @brief Sets a foreground gradient using RGB colors for a specified section of the sprite.
 * 
 * This method sets a foreground gradient using RGB colors between multiple colors.
 * The gradient will be applied to the sprite starting at the given position and
 * extending for the given width and height. The direction of the gradient will
 * be determined by the `direction` parameter. The colors should be specified as
 * RGB values in the format `0xRRGGBB`. Non-integer values will not be accepted.
 * 
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param direction The direction of the gradient.
 * @param numColors The number of colors in the gradient.
 * @param ... A variable list of RGB colors in the gradient as an integer.
 * @return 1 if the gradient was set, 0 if an error occurred.
 */
int Sprite_setForegroundGradient(CmdFX_Sprite* sprite, int x, int y, int width, int height, enum CmdFX_GradientDirection direction, int numColors, ...);

/**
 * @brief Sets a foreground gradient using RGB colors.
 * 
 * This method sets a foreground gradient using RGB colors between multiple colors.
 * The gradient will be applied to the entire sprite. The direction of the gradient
 * will be determined by the `direction` parameter. The colors should be specified as
 * RGB values in the format `0xRRGGBB`. Non-integer values will not be accepted.
 * 
 * @param sprite The sprite to modify.
 * @param direction The direction of the gradient.
 * @param numColors The number of colors in the gradient.
 * @param ... A variable list of RGB colors in the gradient as an integer.
 * @return 1 if the gradient was set, 0 if an error occurred.
 */
int Sprite_setForegroundGradientAll(CmdFX_Sprite* sprite, enum CmdFX_GradientDirection direction, int numColors, ...);

/**
 * @brief Sets a background gradient using RGB colors for a specified section of the sprite.
 * 
 * This method sets a background gradient using RGB colors between multiple colors.
 * The gradient will be applied to the sprite starting at the given position and
 * extending for the given width and height. The direction of the gradient will
 * be determined by the `direction` parameter. The colors should be specified as
 * RGB values in the format `0xRRGGBB`. Non-integer values will not be accepted.
 * 
 * @param sprite The sprite to modify.
 * @param x The X position in the sprite.
 * @param y The Y position in the sprite.
 * @param width The width of the gradient.
 * @param height The height of the gradient.
 * @param direction The direction of the gradient.
 * @param numColors The number of colors in the gradient.
 * @param ... A variable list of RGB colors in the gradient as an integer.
 * @return 1 if the gradient was set, 0 if an error occurred.
 */
int Sprite_setBackgroundGradient(CmdFX_Sprite* sprite, int x, int y, int width, int height, enum CmdFX_GradientDirection direction, int numColors, ...);

/**
 * @brief Sets a background gradient using RGB colors.
 * 
 * This method sets a background gradient using RGB colors between multiple colors.
 * The gradient will be applied to the entire sprite. The direction of the gradient
 * will be determined by the `direction` parameter. The colors should be specified as
 * RGB values in the format `0xRRGGBB`. Non-integer values will not be accepted.
 * 
 * @param sprite The sprite to modify.
 * @param direction The direction of the gradient.
 * @param numColors The number of colors in the gradient.
 * @param ... A variable list of RGB colors in the gradient as an integer.
 * @return 1 if the gradient was set, 0 if an error occurred.
 */
int Sprite_setBackgroundGradientAll(CmdFX_Sprite* sprite, enum CmdFX_GradientDirection direction, int numColors, ...);

#ifdef __cplusplus
}
#endif