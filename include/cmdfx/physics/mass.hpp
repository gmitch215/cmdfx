/**
 * @file mass.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for mass-related physics declarations
 * @version 0.2.1
 * @date 2025-05-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C"
{
#include "cmdfx/physics/mass.h"
}

#include "cmdfx/core/sprites.hpp"

namespace CmdFX
{

/**
 * @brief A C++ wrapper around a CmdFX_SpriteMass struct.
 *
 * This class is a wrapper around the Sprite's mass declarations. It provides
 * methods to get and set the mass of a sprite, as well as to reset it to
 * its default value. The destructor will reset the mass when the object is
 * destroyed.
 */
class SpriteMass final {
  private:
    std::unique_ptr<Sprite> sprite;

  public:
    SpriteMass(std::unique_ptr<Sprite> sprite) : sprite(std::move(sprite)) {
    }
    SpriteMass(CmdFX_Sprite* sprite)
        : sprite(std::make_unique<Sprite>(sprite)) {
    }
    ~SpriteMass() {
        if (sprite) {
            Sprite_resetMass(sprite->getSprite());
        }
    }

    double getDefaultMass(Sprite& sprite) {
        return Sprite_getDefaultMass(sprite.getSprite());
    }

    double getMass(Sprite& sprite) {
        return Sprite_getMass(sprite.getSprite());
    }

    int setMass(Sprite& sprite, double mass) {
        return Sprite_setMass(sprite.getSprite(), mass);
    }

    int resetMass(Sprite& sprite) {
        return Sprite_resetMass(sprite.getSprite());
    }
};
} // namespace CmdFX