/**
 * @file motion.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for velocity and acceleration functions
 * @version 0.2.1
 * @date 2025-05-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C"
{
#include "cmdfx/physics/motion.h"
}

#include "cmdfx/core/sprites.hpp"
#include <memory>
#include <vector>

namespace CmdFX
{

/**
 * @brief A C++ wrapper around a CmdFX_Sprite struct for motion-related
 * functions.
 *
 * This class provides methods to get and set the velocity and acceleration of a
 * sprite, check for potential collisions, and reset the motion of a sprite. The
 * destructor will reset all motion when the object is destroyed.
 */
class SpriteMotion final {
  private:
    std::unique_ptr<Sprite> sprite;

  public:
    SpriteMotion(std::unique_ptr<Sprite> sprite) : sprite(std::move(sprite)) {
    }
    SpriteMotion(CmdFX_Sprite* sprite)
        : sprite(std::make_unique<Sprite>(sprite)) {
    }
    ~SpriteMotion() {
        if (sprite) {
            Sprite_resetAllMotion(sprite->getSprite());
        }
    }

    /**
     * @brief Gets the motion array of a sprite as a vector.
     *
     * The motion array is the internal array that stores the velocity and
     * acceleration of a sprite. The motion array is a 4-element array, where
     * the elements are:
     *
     * - 0: X velocity
     *
     * - 1: Y velocity
     *
     * - 2: X acceleration
     *
     * - 3: Y acceleration
     *
     * The motion array is used by the physics engine to calculate the motion of
     * a sprite. The array is freed from `Sprite_resetAllMotion` and
     * `Sprite_free`, so it should not be freed manually.
     *
     * @param sprite The sprite to use.
     * @return A vector containing the motion values, or an empty vector if the
     * sprite has no motion array or an error occurred.
     */
    std::vector<double> getMotion() {
        std::vector<double> motion;
        motion.reserve(4);

        double* arr = Sprite_getMotion(sprite->getSprite());
        if (arr) {
            for (int i = 0; i < 4; ++i) {
                motion.push_back(arr[i]);
            }
        }

        return motion;
    }

    double getVelocityX() {
        return Sprite_getVelocityX(sprite->getSprite());
    }

    int setVelocityX(double velocity) {
        return Sprite_setVelocityX(sprite->getSprite(), velocity);
    }

    double getVelocityY() {
        return Sprite_getVelocityY(sprite->getSprite());
    }

    int setVelocityY(double velocity) {
        return Sprite_setVelocityY(sprite->getSprite(), velocity);
    }

    double getAccelerationX() {
        return Sprite_getAccelerationX(sprite->getSprite());
    }

    int setAccelerationX(double acceleration) {
        return Sprite_setAccelerationX(sprite->getSprite(), acceleration);
    }

    double getAccelerationY() {
        return Sprite_getAccelerationY(sprite->getSprite());
    }

    int setAccelerationY(double acceleration) {
        return Sprite_setAccelerationY(sprite->getSprite(), acceleration);
    }

    bool isAboutToCollide(Sprite& sprite2) {
        return Sprite_isAboutToCollide(
                   sprite->getSprite(), sprite2.getSprite()
               ) != 0;
    }

    bool isAboutToCollide(const std::unique_ptr<Sprite>& sprite2) {
        return sprite2 && Sprite_isAboutToCollide(
                              sprite->getSprite(), sprite2->getSprite()
                          ) != 0;
    }

    std::vector<std::unique_ptr<Sprite>> getAboutToCollideSprites() {
        std::vector<std::unique_ptr<Sprite>> sprites;
        CmdFX_Sprite** arr =
            Sprite_getAboutToCollideSprites(sprite->getSprite());
        if (arr) {
            for (int i = 0; arr[i] != nullptr; ++i) {
                sprites.emplace_back(std::make_unique<Sprite>(arr[i]));
            }
        }
        return sprites;
    }

    int resetAllMotion() {
        return Sprite_resetAllMotion(sprite->getSprite());
    }
};

/**
 * @brief A C++ wrapper around the CmdFX Physics Engine.
 */
namespace Engine
{

/**
 * @brief Checks if motion debugging is enabled.
 *
 * If motion debugging is enabled, the physics engine will print
 * all sprite motion updates to the console at (3, <sprite id + 1>).
 *
 * @return true if motion debugging is enabled, false otherwise.
 */
bool isMotionDebugEnabled() {
    return Engine_isMotionDebugEnabled() != 0;
}

/**
 * @brief Enables motion debugging.
 *
 * This method enables motion debugging, which will print all
 * sprite motion updates to the console at (3, <sprite id + 1>).
 */
void enableMotionDebug() {
    Engine_enableMotionDebug();
}

/**
 * @brief Disables motion debugging.
 */
void disableMotionDebug() {
    Engine_disableMotionDebug();
}
} // namespace Engine

} // namespace CmdFX