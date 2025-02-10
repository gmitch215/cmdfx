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

// Built-in Forces

/**
 * @brief Gets the friction coefficient of a sprite.
 * 
 * The friction coefficient is a value between 0 and 1 that determines
 * how much friction is applied to a sprite. A value of 0 means no friction,
 * while a value of 1 means full friction, meaning it will stop immediately.
 * 
 * @param sprite The sprite to use.
 * @return The friction coefficient of the sprite.
 */
double Sprite_getFrictionCoefficient(CmdFX_Sprite* sprite);

/**
 * @brief Sets the friction coefficient of a sprite.
 * 
 * The friction coefficient is a value between 0 and 1 that determines
 * how much friction is applied to a sprite. A value of 0 means no friction,
 * while a value of 1 means full friction, meaning it will stop immediately.
 * 
 * @param sprite The sprite to use.
 * @param coefficient The friction coefficient of the sprite.
 * @return The friction coefficient of the sprite.
 */
double Sprite_setFrictionCoefficient(CmdFX_Sprite* sprite, double coefficient);

/**
 * @brief Gets the friction force acting on a sprite.
 * 
 * The friction force is the force acting against the direction of motion
 * of a sprite. It is calculated as the product of the friction coefficient
 * and the normal force acting on the sprite.
 * 
 * Friction only applies if the sprite is on the ground. If the sprite is
 * under or over the ground, friction will not apply.
 * 
 * @param sprite The sprite to use.
 * @return The friction force acting on the sprite.
 */
CmdFX_Vector* Sprite_getFrictionForce(CmdFX_Sprite* sprite);

/**
 * @brief Gets the normal force acting on a sprite.
 * 
 * The normal force is the force acting perpendicular to the ground
 * on a sprite. It is calculated as the product of the mass of the sprite
 * and the acceleration due to gravity.
 * 
 * @param sprite The sprite to use.
 * @return The normal force acting on the sprite.
 */
CmdFX_Vector* Sprite_getNormalForce(CmdFX_Sprite* sprite);

#ifdef __cplusplus
}
#endif