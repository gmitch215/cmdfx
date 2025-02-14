/**
 * @file force.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Force-related physics declarations
 * @version 0.1.1
 * @date 2025-02-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include "cmdfx/core/sprites.h"
#include "cmdfx/physics/util.h"

#ifdef __cplusplus
extern "C" {
#endif

// Base Force Functions

/**
 * @brief Gets all of the forces acting on a sprite.
 * 
 * @param sprite The sprite to use.
 * @return The forces acting on the sprite as a vector array.
 */
CmdFX_Vector** Sprite_getAllForces(CmdFX_Sprite* sprite);

/**
 * @brief Gets the net force acting on a sprite.
 * 
 * @param sprite The sprite to use.
 * @return The net force acting on the sprite.
 */
CmdFX_Vector* Sprite_getNetForce(CmdFX_Sprite* sprite);

/**
 * @brief Adds a force to a sprite.
 * 
 * @param sprite The sprite to use.
 * @param force The force to add.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_addForce(CmdFX_Sprite* sprite, CmdFX_Vector* force);

/**
 * @brief Removes a force from a sprite.
 * 
 * The force will be removed if it is found in the sprite's force array.
 * 
 * @param sprite The sprite to use.
 * @param force The force to remove.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_removeForce(CmdFX_Sprite* sprite, CmdFX_Vector* force);

/**
 * @brief Removes all forces from a sprite.
 * 
 * @param sprite The sprite to use.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_removeAllForces(CmdFX_Sprite* sprite);

#ifdef __cplusplus
}
#endif