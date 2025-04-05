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
 * @brief Gets the number of forces acting on a sprite.
 * 
 * @param sprite The sprite to use.
 * @return The number of forces acting on the sprite.
 */
int Sprite_getAllForcesCount(CmdFX_Sprite* sprite);

/**
 * @brief Gets the net force acting on a sprite.
 * 
 * This creates a new vector that is the sum of all the forces acting on the sprite.
 * The vector should be freed after use.
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
 * The force will be removed if it is found in the sprite's force array and
 * will not be freed.
 * 
 * @param sprite The sprite to use.
 * @param force The force to remove.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_removeForce(CmdFX_Sprite* sprite, CmdFX_Vector* force);

/**
 * @brief Removes all forces from a sprite.
 * 
 * Removing all forces will free all forces in the sprite's force array,
 * and then free the array itself.
 * 
 * @param sprite The sprite to use.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_removeAllForces(CmdFX_Sprite* sprite);

/**
 * @brief Removes all forces for every sprite and frees the array.
 * 
 * This will free all forces in the sprite's force array, and then free
 * the array itself.
 * 
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_clearAllForces();

/**
 * @brief Gets the friction coefficient of a sprite.
 * 
 * The friction coefficient is a value between 0 and 1 that determines
 * how much a sprite will slow down when moving on the ground.
 * 
 * If the sprite does not have a custom friction coefficient, the default
 * value of `Engine_getDefaultFrictionCoefficient` will be returned.
 * 
 * @param sprite The sprite to use.
 * @return The friction coefficient of the sprite.
 */
double Sprite_getFrictionCoefficient(CmdFX_Sprite* sprite);

/**
 * @brief Sets the friction coefficient of a sprite.
 * 
 * The friction coefficient is a value between 0 and 1 that determines
 * how much a sprite will slow down when moving on the ground.
 * 
 * @param sprite The sprite to use.
 * @param coefficient The friction coefficient to set.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_setFrictionCoefficient(CmdFX_Sprite* sprite, double coefficient);

/**
 * @brief Resets the friction coefficient of a sprite.
 * 
 * The friction coefficient will be reset to the default value.
 * 
 * @param sprite The sprite to use.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_resetFrictionCoefficient(CmdFX_Sprite* sprite);

// Impulse Functions

/**
 * @brief Adds a temporary force to a sprite.
 * 
 * @param sprite The sprite to use.
 * @param force The force to add.
 * @param duration The duration of the force, in milliseconds.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_addForceFor(CmdFX_Sprite* sprite, CmdFX_Vector* force, int duration);

#ifdef __cplusplus
}
#endif