/**
 * @file force.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the CmdFX Force API
 * @version 0.2.1
 * @date 2025-05-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C"
{
#include "cmdfx/physics/force.h"
}

#include "cmdfx/core/sprites.hpp"
#include "cmdfx/physics/util.hpp"
#include <vector>

namespace CmdFX
{

/**
 * @brief A C++ wrapper around Sprite forces.
 */
namespace SpriteForces
{

/**
 * @brief Gets all the forces acting on a sprite.
 * @param sprite The sprite to use.
 * @return A vector of Vector objects representing all forces acting on the
 * sprite.
 */
std::vector<Vector> getAllForces(Sprite& sprite) {
    CmdFX_Vector** forces = Sprite_getAllForces(sprite.getSprite());
    int count = Sprite_getAllForcesCount(sprite.getSprite());

    std::vector<Vector> result;
    result.reserve(count);

    for (int i = 0; i < count; i++) {
        result.emplace_back(forces[i]);
    }

    return result;
}

Vector getNetForce(Sprite& sprite) {
    CmdFX_Vector* netForce = Sprite_getNetForce(sprite.getSprite());
    return Vector(netForce);
}

int addForce(Sprite& sprite, Vector& force) {
    return Sprite_addForce(sprite.getSprite(), force.getVector());
}

int addForceFor(Sprite& sprite, Vector& force, int duration) {
    return Sprite_addForceFor(sprite.getSprite(), force.getVector(), duration);
}

int removeForce(Sprite& sprite, Vector& force) {
    return Sprite_removeForce(sprite.getSprite(), force.getVector());
}

int removeAllForces(Sprite& sprite) {
    return Sprite_removeAllForces(sprite.getSprite());
}

static int clearAllForces() {
    return Sprite_clearAllForces();
}

double getFrictionCoefficient(Sprite& sprite) {
    return Sprite_getFrictionCoefficient(sprite.getSprite());
}

int setFrictionCoefficient(Sprite& sprite, double coefficient) {
    return Sprite_setFrictionCoefficient(sprite.getSprite(), coefficient);
}

int resetFrictionCoefficient(Sprite& sprite) {
    return Sprite_resetFrictionCoefficient(sprite.getSprite());
}

}; // namespace SpriteForces

} // namespace CmdFX