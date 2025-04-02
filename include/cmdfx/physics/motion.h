/**
 * @file motion.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Velocity and Acceleration functions
 * @version 0.1.4
 * @date 2025-04-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include "cmdfx/core/sprites.h"

/**
 * @brief Gets the motion array of a sprite.
 * 
 * The motion array is the internal array that stores the velocity and acceleration of a sprite.
 * The motion array is a 4-element array, where the elements are:
 * 
 * - 0: X velocity
 * 
 * - 1: Y velocity
 * 
 * - 2: X acceleration
 * 
 * - 3: Y acceleration
 * 
 * The motion array is used by the physics engine to calculate the motion of a sprite.
 * The array is freed from `Sprite_resetAllMotion` and `Sprite_free`, so it should not be freed manually.
 * 
 * @param sprite The sprite to use.
 * @return The motion array of the sprite, or `0` if the sprite has no motion array or an error occurred.
 */
int* Sprite_getMotion(CmdFX_Sprite* sprite);

/**
 * @brief Gets the X velocity of a sprite.
 * @param sprite The sprite to use.
 * @return The X velocity of the sprite.
 */
int Sprite_getVelocityX(CmdFX_Sprite* sprite);

/**
 * @brief Sets the X velocity of a sprite.
 * @param sprite The sprite to use.
 * @param velocity The X velocity to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_setVelocityX(CmdFX_Sprite* sprite, int velocity);

/**
 * @brief Gets the Y velocity of a sprite.
 * @param sprite The sprite to use.
 * @return The Y velocity of the sprite.
 */
int Sprite_getVelocityY(CmdFX_Sprite* sprite);

/**
 * @brief Sets the Y velocity of a sprite.
 * @param sprite The sprite to use.
 * @param velocity The Y velocity to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_setVelocityY(CmdFX_Sprite* sprite, int velocity);

/**
 * @brief Gets the X acceleration of a sprite.
 * @param sprite The sprite to use.
 * @return The X acceleration of the sprite.
 */
int Sprite_getAccelerationX(CmdFX_Sprite* sprite);

/**
 * @brief Sets the X acceleration of a sprite.
 * @param sprite The sprite to use.
 * @param acceleration The X acceleration to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_setAccelerationX(CmdFX_Sprite* sprite, int acceleration);

/**
 * @brief Gets the Y acceleration of a sprite.
 * @param sprite The sprite to use.
 * @return The Y acceleration of the sprite.
 */
int Sprite_getAccelerationY(CmdFX_Sprite* sprite);

/**
 * @brief Sets the Y acceleration of a sprite.
 * @param sprite The sprite to use.
 * @param acceleration The Y acceleration to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_setAccelerationY(CmdFX_Sprite* sprite, int acceleration);

/**
 * @brief Resets all motion of a sprite.
 * 
 * This method resets all motion of a sprite, including its velocity and
 * acceleration. The sprite will be set to its default state, with no
 * motion applied.
 * 
 * @param sprite The sprite to use.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_resetAllMotion(CmdFX_Sprite* sprite);

/**
 * @brief Applies the motion of a sprite.
 * 
 * This method applies the motion of a sprite by updating its position
 * based on its velocity and acceleration. The motion is applied in the
 * opposite direction of the Y axis, so the sprite will move up when
 * the Y velocity is positive and down when the Y velocity is negative.
 * 
 * @param sprite The sprite to use.
 */
void Engine_applyMotion(CmdFX_Sprite* sprite);