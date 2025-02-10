/**
 * @file sprites.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Animation Extensions for sprites.h
 * @version 0.1.0
 * @date 2025-01-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include "cmdfx/core/sprites.h"

#ifdef __cplusplus
extern "C" {
#endif

// Utility Methods - Movement

/**
 * @brief Moves a sprite to the given position with an animation.
 * 
 * This method moves a sprite to the given position with an animation. The sprite
 * will move from its current position to the given position over the specified
 * time. The time is in seconds, and the sprite will move at a constant speed.
 * The sprite will be redrawn at each position during the animation.
 * 
 * @param sprite The sprite to move.
 * @param x The new X position of the sprite.
 * @param y The new Y position of the sprite.
 * @param time The time, in seconds, for the animation.
 */
void Sprite_moveTo_anim(CmdFX_Sprite* sprite, int x, int y, double time);

/**
 * @brief Moves a sprite by the given amount with an animation.
 * 
 * This method moves a sprite by the given amount with an animation. The sprite
 * will move by the given amount over the specified time. The time is in seconds,
 * and the sprite will move at a constant speed. The sprite will be redrawn at
 * each position during the animation.
 * 
 * @param sprite The sprite to move.
 * @param dx The delta X position to move the sprite by.
 * @param dy The delta Y position to move the sprite by.
 * @param time The time, in seconds, for the animation.
 */
void Sprite_moveBy_anim(CmdFX_Sprite* sprite, int dx, int dy, double time);

#ifdef __cplusplus
}
#endif