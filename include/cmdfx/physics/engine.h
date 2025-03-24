/**
 * @file engine.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Global physics engine declarations
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
 * @brief Gets whether this sprite is static.
 * 
 * A sprite is static if it is not affected by the physics engine.
 * 
 * @param sprite The sprite to check.
 * @return 1 if the sprite is static, 0 otherwise.
 */
int Sprite_isStatic(CmdFX_Sprite* sprite);

/**
 * @brief Sets whether this sprite is static.
 * 
 * A sprite is static if it is not affected by the physics engine.
 * Use this method to set whether a sprite is static or not.
 * 
 * @param sprite The sprite to modify.
 * @param isStatic 1 if the sprite is static, 0 otherwise.
 * @return 0 if successful, -1 if an error occurred.
 */
int Sprite_setStatic(CmdFX_Sprite* sprite, int isStatic);

// Engine Declarations

/**
 * @brief Gets the force of gravity, in characters per second squared.
 * 
 * This method returns the force of gravity, in characters per second squared.
 * The default value is 1 c/s^2.
 * 
 * @return The force of gravity, in c/s^2.
 */
int Engine_getForceOfGravity();

/**
 * @brief Sets the force of gravity, in characters per second squared.
 * 
 * This method sets the force of gravity, in characters per second squared.
 * The default value is 1 c/s^2.
 * 
 * @param force The force of gravity, in c/s^2.
 * @return 0 if successful, -1 if an error occurred.
 */
int Engine_setForceOfGravity(int force);

/**
 * @brief Gets the terminal velocity, in characters per second.
 * 
 * This method returns the terminal velocity, in characters per second.
 * The default value is 10 c/s.
 * 
 * @return The terminal velocity, in c/s.
 */
int Engine_getTerminalVelocity();

/**
 * @brief Sets the terminal velocity, in characters per second.
 * 
 * This method sets the terminal velocity, in characters per second.
 * The default value is 10 c/s.
 * 
 * @param velocity 
 * @return 0 if successful, -1 if an error occurred.
 */
int Engine_setTerminalVelocity(int velocity);

/**
 * @brief Gets the Y level of the ground for the physics engine.
 * 
 * The default value is the canvas height, because (0, 0) refers to the top
 * left of the terminal. The Y level of the ground helps determine when a 
 * sprite has landed on the ground.
 * 
 * @return The Y level of the ground.
 */
int Engine_getGroundY();

/**
 * @brief Sets the Y level of the ground for the physics engine.
 * 
 * The default value is the canvas height. The Y level of the ground helps 
 * determine when a sprite has landed on the ground.
 * 
 * @param y The Y level of the ground.
 * @return 0 if successful, -1 if an error occurred.
 */
int Engine_setGroundY(int y);

// Engine Defaults

/**
 * @brief Gets the default friction coefficient for a sprite.
 * 
 * The default friction coefficient is a value between 0 and 1 that determines
 * how much friction is applied to a sprite. A value of 0 means no friction,
 * while a value of 1 means full friction, meaning it will stop immediately.
 * 
 * The default value is 0.25.
 * 
 * @return The default friction coefficient.
 */
double Engine_getDefaultFrictionCoefficient();

/**
 * @brief Sets the default friction coefficient for a sprite.
 * 
 * The default friction coefficient is a value between 0 and 1 that determines
 * how much friction is applied to a sprite. A value of 0 means no friction,
 * while a value of 1 means full friction, meaning it will stop immediately.
 * 
 * The default value is 0.25.
 * 
 * @param coefficient The default friction coefficient.
 * @return 0 if successful, -1 if an error occured.
 */
int Engine_setDefaultFrictionCoefficient(double coefficient);

/**
 * @brief Gets the default mass of a character.
 * 
 * The mass of a sprite is calculated by the number of non-whitespace 
 * characters in a sprite map. ANSI codes have no effect on the mass 
 * of a character.
 * 
 * You can modify the output value of `Sprite_getDefaultMass` by
 * changing the default mass of a character. Every character by default
 * has a mass of 1.
 * 
 * @return The default mass of a character.
 */
int Engine_getCharacterMass(char c);

/**
 * @brief Sets the default mass of a character.
 * 
 * The mass of a sprite is calculated by the number of non-whitespace 
 * characters in a sprite map. ANSI codes have no effect on the mass 
 * of a character.
 * 
 * You can modify the output value of `Sprite_getDefaultMass` by
 * changing the default mass of a character. Every character by default
 * has a mass of 1.
 * 
 * @param c The character to set the mass of.
 * @param mass The default mass of the character.
 * @return 0 if successful, -1 if an error occured.
 */
int Engine_setCharacterMass(char c, int mass);

// Engine

/**
 * @brief Starts up the physics engine.
 * 
 * @return 0 if successful, -1 if an error occured.
 */
int Engine_start();

/**
 * @brief Ticks the physics engine.
 * 
 * 
 * This method ticks the physcis engine, if it is running.
 * This is automatically called once per frame.
 */
void Engine_tick();

/**
 * @brief Cleans up the physics engine.
 * 
 * This is different from `Engine_end`, in that it claens up
 * any loose variables before stopping the engine. This
 * is automatically called by `Engine_end` and is not intended
 * to be called independently to avoid undefined behavior.
 * 
 * @return 0 if successful, -1 if an error occured.
 */
int Engine_cleanup();

/**
 * @brief Stops the physics engine.
 * 
 * @return 0 if successful, -1 if an error occured.
 */
int Engine_end();

#ifdef __cplusplus
}
#endif