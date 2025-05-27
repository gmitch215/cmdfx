/**
 * @file engine.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the CmdFX Physics Engine
 * @version 0.2.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once 

extern "C" {
    #include "cmdfx/physics/engine.h"
}

#include "cmdfx/core/sprites.hpp"
#include "cmdfx/physics/motion.hpp"

namespace CmdFX {

    /**
     * @brief A C++ wrapper around the CmdFX Physics Engine.
     */
    namespace Engine {
        int start() {
            return Engine_start();
        }

        /**
         * @brief Ticks the physics engine and returns modified sprites.
         * 
         * This method ticks the physics engine, which processes all sprites
         * and applies physics calculations to them. It returns a vector of
         * unique_ptr<Sprite> objects that were modified during the tick.
         * 
         * @return A vector of unique_ptr<Sprite> containing all modified sprites.
         */
        std::vector<std::unique_ptr<Sprite>> tick() {
            CmdFX_Sprite** modifiedSprites = Engine_tick();
            std::vector<std::unique_ptr<Sprite>> sprites;

            for (int i = 0; modifiedSprites[i] != nullptr; i++) {
                sprites.emplace_back(std::make_unique<Sprite>(modifiedSprites[i]));
            }

            return sprites;
        }

        void cleanup() {
            Engine_cleanup();
        }

        bool isMotionDebugEnabled() {
            return Engine_isMotionDebugEnabled() != 0;
        }

        bool isSpriteStatic(Sprite& sprite) {
            return Sprite_isStatic(sprite.getSprite()) != 0;
        }

        void setSpriteStatic(Sprite& sprite, bool isStatic) {
            Sprite_setStatic(sprite.getSprite(), isStatic ? 1 : 0);
        }

        double getForceOfGravity() {
            return Engine_getForceOfGravity();
        }

        void setForceOfGravity(double force) {
            Engine_setForceOfGravity(force);
        }

        double getTerminalVelocity() {
            return Engine_getTerminalVelocity();
        }

        void setTerminalVelocity(double velocity) {
            Engine_setTerminalVelocity(velocity);
        }

        int getGroundY() {
            return Engine_getGroundY();
        }

        void setGroundY(int y) {
            Engine_setGroundY(y);
        }

        double getDefaultFrictionCoefficient() {
            return Engine_getDefaultFrictionCoefficient();
        }

        void setDefaultFrictionCoefficient(double coefficient) {
            Engine_setDefaultFrictionCoefficient(coefficient);
        }

        double getCharacterMass(char c) {
            return Engine_getCharacterMass(c);
        }

        void setCharacterMass(char c, double mass) {
            Engine_setCharacterMass(c, mass);
        }
    };

}