/**
 * @file costumes.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Sprite Costumes API for managing sprite states.
 * @version 0.1.3
 * @date 2025-03-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "cmdfx/core/sprites.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The maximum amount of costumes a sprite can have.
 * 
 * This defines the maximum number of costumes a sprite can have in its
 * SpriteCostumes holder.
 */
#define MAX_SPRITE_COSTUMES 128

/**
 * @brief Represents the holder for sprite costumes.
 * 
 * This struct is used to hold sprite costumes. A sprite costume is a state
 * of a sprite. For example, a sprite costume could be a sprite with a different
 * color, or a sprite with a different shape.
 * 
 * This is automatically constructed and freed along with the sprite using
 * `Sprite_create` and `Sprite_free`.
 */
typedef struct CmdFX_SpriteCostumes {
    /**
     * @brief Represents the sprite costumes.
     * 
     * This is a 3D array of characters. The first dimension is the costume
     * index, the second dimension is the row, and the third dimension is the
     * column.
     * 
     * This array must match the same length as the `ansiCostumes` array. `costumeCount`
     * must be the same length as the first dimension of this array.
     * 
     * There is an internal limit of 128 costumes per sprite.
     */
    char*** costumes;
    /**
     * @brief Represents the ANSI codes for the sprite costumes.
     * 
     * This is a 3D array of strings. The first dimension is the costume index,
     * the second dimension is the row, and the third dimension is the ANSI code.
     * 
     * This array must match the same length as the `costumes` array. `costumeCount`
     * must be the same length as the first dimension of this array. ANSI costumes
     * can be initialized to 0 to account for no ANSI codes.
     * 
     * There is an internal limit of 128 costumes per sprite.
     */
    char**** ansiCostumes;
    /**
     * @brief Represents the number of sprite costumes.
     * 
     * This is the number of sprite costumes in the `costumes` and the
     * `ansiCostumes` arrays.
     * 
     * There is an internal limit of 128 costumes per sprite.
     */
    int costumeCount;
} CmdFX_SpriteCostumes;

/**
 * @brief Creates a sprite costumes holder.
 * 
 * This method creates a sprite costumes holder with a specific number of
 * costumes. The costumes are automatically freed with `Sprite_free`.
 * 
 * The default costume is the current sprite data itself at index 0.
 * If `costumeCount` is less than 1, the method will return `NULL`.
 * If `costumeCount` is greater 1, the rest of the costumes will be
 * initialized to 0.
 * 
 * There is an internal limit of 128 costumes per sprite.
 * 
 * @param sprite The sprite to create the costume for.
 * @param costumeCount The number of costumes to create.
 * @return The sprite costumes, or `NULL` if an error occurred.
 */
CmdFX_SpriteCostumes* Sprite_createCostumes(CmdFX_Sprite* sprite, int costumeCount);

/**
 * @brief Gets the sprite costumes.
 * 
 * This method gets the sprite costumes from a sprite. It needs to be
 * created if not already created, but is automatically freed with 
 * `Sprite_free`.
 * 
 * The default costume is the current sprite data itself at index 0.
 * 
 * @param sprite The sprite to get the costumes for.
 * @return The sprite costumes, or 0 if not created.
 */
CmdFX_SpriteCostumes* Sprite_getCostumes(CmdFX_Sprite* sprite);

/**
 * @brief Gets the sprite costume at a specific index.
 * 
 * This method gets the sprite costume at a specific index. If the index
 * is out of bounds, the method will return `NULL`.
 * 
 * @param sprite The sprite to get the costume for.
 * @param costumeIndex The index of the costume.
 * @return The sprite costume, or `NULL` if not valid.
 */
char** Sprite_getCharCostume(CmdFX_Sprite* sprite, int costumeIndex);

/**
 * @brief Gets the ANSI costume for a sprite.
 * 
 * This method gets the ANSI costume for a sprite. If the costume index
 * is out of bounds, the method will return `NULL`.
 * 
 * @param sprite The sprite to get the ANSI costume for.
 * @param costumeIndex The index of the costume.
 * @return The ANSI costume, or `NULL` if not valid.
 */
char*** Sprite_getAnsiCostume(CmdFX_Sprite* sprite, int costumeIndex);

/**
 * @brief Switches the sprite costume to a specific index.
 * 
 * This method switches the sprite costume to a specific index. If the
 * index is out of bounds, the method will return -1. The costume
 * will switch the `data` and the `ansiData` inside the sprite, and
 * set the old data and ANSI data to the costume index.
 * 
 * @param sprite The sprite to switch the costume for.
 * @param costumeIndex The index of the costume to switch to.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_switchCostumeTo(CmdFX_Sprite* sprite, int costumeIndex);

/**
 * @brief Adds a sprite costume.
 * 
 * This method adds a sprite costume to the sprite. The costume will be
 * added to the `costumes` and the `ansiCostumes` arrays. If the costume
 * already exists, the method will return -1.
 * 
 * @param sprite The sprite to add the costume to.
 * @param costume The costume to add.
 * @param ansiCostume The ANSI code for the costume.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_addCostume(CmdFX_Sprite* sprite, char** costume, char*** ansiCostume);

/**
 * @brief Removes a sprite costume.
 * 
 * This method removes a sprite costume from the sprite. The costume will
 * be removed from the `costumes` and the `ansiCostumes` arrays. If the
 * costume does not exist, the method will return -1.
 * 
 * @param sprite The sprite to remove the costume from.
 * @param costumeIndex The index of the costume to remove.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_removeCostume(CmdFX_Sprite* sprite, int costumeIndex);

/**
 * @brief Resets the sprite costumes.
 * 
 * This method resets the sprite costumes to the default costume, and
 * removes all of the other costumes. The default costume is the current 
 * sprite data itself at index 0 that it was initialized with.
 * 
 * @param sprite The sprite to reset the costumes for.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_resetCostumes(CmdFX_Sprite* sprite);

#ifdef __cplusplus
}
#endif