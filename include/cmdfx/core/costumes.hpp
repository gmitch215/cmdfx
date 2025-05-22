/**
 * @file costumes.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the CmdFX Costumes API
 * @version 0.2.1
 * @date 2025-05-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

extern "C" {
    #include "cmdfx/core/costumes.h"
}

#include "cmdfx/core/sprites.hpp"
#include <string>
#include <vector>
#include <memory>

namespace CmdFX {

    /**
     * @brief A C++ wrapper around a CmdFX_SpriteCostumes struct.
     * 
     * This class is a wrapper around the CmdFX_SpriteCostumes struct. It provides
     * a constructor and destructor for the sprite costumes, as well as a method
     * to get the sprite costumes. The destructor will free the sprite
     * costumes' memory when the object is destroyed.
     */
    class SpriteCostumes final {
        private:
            std::unique_ptr<Sprite> sprite;
            CmdFX_SpriteCostumes* costumes;

        public:
            SpriteCostumes(CmdFX_SpriteCostumes* costumes) : costumes(costumes) {}
            SpriteCostumes(std::unique_ptr<Sprite> sprite, int count) : sprite(std::move(sprite)) {
                costumes = Sprite_createCostumes(this->sprite->getSprite(), count);
            }

            ~SpriteCostumes() {
                if (costumes) { Sprite_freeCostumes(sprite->getSprite()); }
            }

            CmdFX_SpriteCostumes* getSpriteCostumes() { return costumes; }

            int getCount() { return costumes->costumeCount; }

            void setCostume(int index, char** text, char*** ansi) {
                Sprite_setCostumeAt(sprite->getSprite(), index, text, ansi);
            }

            void setCostume(int index, std::vector<std::string> text, std::vector<std::vector<std::string>> ansi) {
                char** textArray = to2DArray(text);
                char*** ansiArray = to3DArray(ansi);

                Sprite_setCostumeAt(sprite->getSprite(), index, textArray, ansiArray);
            }
    };

}