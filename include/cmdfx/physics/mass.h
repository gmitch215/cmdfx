/**
 * @file mass.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Mass-related physics declarations
 * @version 0.1.1
 * @date 2025-02-09
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
 * @brief Gets the default mass of a sprite.
 *
 * The default mass of a sprite is calculated by the number of
 * non-whitespace characters in a sprite map. ANSI codes have no
 * effect on the mass of a sprite.
 *
 * @param sprite The sprite to use.
 * @return The default mass of the sprite.
 */
double Sprite_getDefaultMass(CmdFX_Sprite* sprite);

/**
 * @brief Gets the mass of a sprite.
 *
 * This will default to `Sprite_getDefaultMass` if a custom mass
 * has not been set.
 *
 * @param sprite The sprite to use.
 * @return The mass of the sprite.
 */
double Sprite_getMass(CmdFX_Sprite* sprite);

/**
 * @brief Sets the mass of a sprite.
 *
 * This method can be used to set custom mass values for a sprite.
 * You can still calculate the original value with
 * `Sprite_getDefaultMass`.
 *
 * @param sprite The sprite to use.
 * @param mass The mass of the sprite.
 * @return The mass of the sprite.
 */
int Sprite_setMass(CmdFX_Sprite* sprite, double mass);

/**
 * @brief Resets the mass of a sprite
 *
 * This method will reset the mass of a sprite to the default
 * value.
 *
 * @param sprite The sprite to use.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_resetMass(CmdFX_Sprite* sprite);

#ifdef __cplusplus
}
#endif