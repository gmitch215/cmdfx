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

typedef struct CmdFX_Sprite {
    /**
     * The X position of the sprite.
     */
    int x;
    /**
     * The Y position of the sprite.
     */
    int y;
    /**
     * The width of the sprite.
     */
    int width;
    /**
     * The height of the sprite.
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
     * The string is prefixed with "\033[" and suffixed with "m". Each row should
     * be the same length, and the number of rows should be equal to the height of
     * the sprite. The sprite will be drawn starting at the X and Y position, with
     * the top-left corner of the sprite at the X and Y position. The number of
     * columns should be equal to the width of the sprite.
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
 * given parameters. The sprite should be freed with `free` when it is no longer
 * needed.
 * 
 * This method does not draw the sprite to the terminal. To draw the sprite, use
 * the `Sprite_draw` method.
 * @param x The X position of the sprite.
 * @param y The Y position of the sprite.
 * @param width The width of the sprite.
 * @param height The height of the sprite.
 * @param text The text data for the sprite.
 * @param ansi The ANSI data for the sprite.
 * @param z The Z-index of the sprite.
 * @return A pointer to the new sprite, or 0 if an error occurred.
 */
CmdFX_Sprite* Sprite_create(int x, int y, int width, int height, char** text, char*** ansi, int z);

/**
 * Draws a sprite to the terminal. If the sprite is already drawn, it will be
 * redrawn at its current position.
 * @param sprite The sprite to draw.
 */
void Sprite_draw(CmdFX_Sprite* sprite);

/**
 * Removes a sprite from the terminal, making it no longer visble.
 * @param sprite The sprite to remove.
 */
void Sprite_remove(CmdFX_Sprite* sprite);

// Utility Methods - Movement

/**
 * Moves a sprite to the given position.
 * @param sprite The sprite to move.
 * @param x The new X position of the sprite.
 * @param y The new Y position of the sprite.
 */
void Sprite_moveTo(CmdFX_Sprite* sprite, int x, int y);

/**
 * Moves a sprite by the given amount.
 * @param sprite The sprite to move.
 * @param dx The delta X position to move the sprite by.
 * @param dy The delta Y position to move the sprite by.
 */
void Sprite_moveBy(CmdFX_Sprite* sprite, int dx, int dy);

// Utility Methods - Collisions

/**
 * Gets the sprites that are colliding with the given sprite.
 * @param sprite The sprite to check for collisions.
 * @return An array of pointers to the colliding sprites.
 */
CmdFX_Sprite** Sprite_getCollidingSprites(CmdFX_Sprite* sprite);

/**
 * Checks if two sprites are colliding.
 * @param sprite1 The first sprite.
 * @param sprite2 The second sprite.
 * @return 1 if the sprites are colliding, 0 otherwise.
 */
int Sprite_isColliding(CmdFX_Sprite* sprite1, CmdFX_Sprite* sprite2);

#ifdef __cplusplus
}
#endif