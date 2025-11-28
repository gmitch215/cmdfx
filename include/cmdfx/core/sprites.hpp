/**
 * @file sprites.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the CmdFX Sprites API
 * @version 0.2.1
 * @date 2025-05-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C" {
#include "cmdfx/core/sprites.h"
}

#include "cmdfx/core/builder.hpp"
#include <memory>
#include <string>
#include <vector>

namespace CmdFX
{

/**
 * @brief A C++ wrapper around a CmdFX_Sprite struct.
 *
 * This class is a wrapper around the CmdFX_Sprite struct. It provides a
 * constructor and destructor for the sprite, as well as a method to get the
 * sprite's data. The destructor will free the sprite's memory when the
 * object is destroyed.
 */
class Sprite final {
  private:
    CmdFX_Sprite* sprite;

  public:
    Sprite(CmdFX_Sprite* sprite) : sprite(sprite) {
    }
    Sprite(char** text, char*** ansi, int z) {
        sprite = Sprite_create(text, ansi, z);
    }
    Sprite(
        std::vector<std::string> text,
        std::vector<std::vector<std::string>> ansi, int z
    ) {
        sprite = Sprite_create(to2DArray(text), to3DArray(ansi), z);
    }

    ~Sprite() {
        if (sprite) {
            Sprite_free(sprite);
        }
    }

    /**
     * @brief Get the Sprite object associated with this class.
     * @return CmdFX_Sprite* The sprite object.
     */
    CmdFX_Sprite* getSprite() {
        return sprite;
    }

    int getX() const {
        return sprite->x;
    }

    int getY() const {
        return sprite->y;
    }

    int getWidth() const {
        return sprite->width;
    }

    int getHeight() const {
        return sprite->height;
    }

    int getZ() const {
        return sprite->z;
    }

    int draw(int x, int y) {
        return Sprite_draw(x, y, sprite);
    }

    void remove() {
        Sprite_remove(sprite);
    }

    /**
     * @brief Get the data of a sprite as a 1D vector of strings.
     * @return The data of the sprite.
     */
    std::vector<std::string> getData() {
        std::vector<std::string> text;
        for (int i = 0; i < sprite->height; i++) {
            text.push_back(std::string(sprite->data[i]));
        }
        return text;
    }

    void setData(std::vector<std::string> text) {
        char** data = to2DArray(text);
        Sprite_setData(sprite, data);
    }

    void setData(char** text) {
        Sprite_setData(sprite, text);
    }

    /**
     * @brief Get the ANSI data of a sprite as a 2D vector of strings.
     * @return The ansi of the sprite.
     */
    std::vector<std::vector<std::string>> getAnsi() {
        std::vector<std::vector<std::string>> ansi;
        for (int i = 0; i < sprite->height; i++) {
            std::vector<std::string> row;
            for (int j = 0; j < sprite->width; j++) {
                row.push_back(std::string(sprite->ansi[i][j]));
            }
            ansi.push_back(row);
        }
        return ansi;
    }

    void setChar(int x, int y, char c) {
        Sprite_setChar(sprite, x, y, c);
    }

    int fillChar(int x, int y, int width, int height, char c) {
        return Sprite_fillChar(sprite, x, y, width, height, c);
    }

    int fillCharEmpty(int x, int y, int width, int height, char c) {
        return Sprite_fillCharEmpty(sprite, x, y, width, height, c);
    }

    int fillCharAll(char c) {
        return Sprite_fillCharAll(sprite, c);
    }

    int fillCharAllEmpty(char c) {
        return Sprite_fillCharAllEmpty(sprite, c);
    }

    int setAnsi(int x, int y, char* ansi) {
        return Sprite_setAnsi(sprite, x, y, ansi);
    }

    int appendAnsi(int x, int y, char* ansi) {
        return Sprite_appendAnsi(sprite, x, y, ansi);
    }

    int fillAnsi(int x, int y, char* ansi, int width, int height) {
        return Sprite_fillAnsi(sprite, x, y, ansi, width, height);
    }

    int setAnsiAll(char* ansi) {
        return Sprite_setAnsiAll(sprite, ansi);
    }

    int appendAnsiAll(char* ansi) {
        return Sprite_appendAnsiAll(sprite, ansi);
    }

    int resize(int width, int height) {
        return Sprite_resize(sprite, width, height);
    }

    int resizeWithPadding(int width, int height, char padding) {
        return Sprite_resizeWithPadding(sprite, width, height, padding);
    }

    int center() {
        return Sprite_center(sprite);
    }

    int resizeAndCenter(int width, int height) {
        return Sprite_resizeAndCenter(sprite, width, height);
    }

    void moveTo(int x, int y) {
        Sprite_moveTo(sprite, x, y);
    }

    void moveBy(int dx, int dy) {
        Sprite_moveBy(sprite, dx, dy);
    }

    std::vector<std::unique_ptr<Sprite>> getCollidingSprites() {
        std::vector<std::unique_ptr<Sprite>> collisions;
        CmdFX_Sprite** collisionSprites = Sprite_getCollidingSprites(sprite);
        for (int i = 0; collisionSprites[i] != nullptr; i++) {
            collisions.push_back(std::make_unique<Sprite>(collisionSprites[i]));
        }
        return collisions;
    }

    bool isCollidingWith(std::unique_ptr<Sprite> other) {
        return Sprite_isColliding(sprite, other->getSprite());
    }

    bool isOnTop(int x, int y) {
        return Sprite_isOnTop(sprite, x, y);
    }

    bool isOnBottom(int x, int y) {
        return Sprite_isOnBottom(sprite, x, y);
    }

    int setForeground(int x, int y, int rgb) {
        return Sprite_setForeground(sprite, x, y, rgb);
    }

    int setForeground(int rgb) {
        return Sprite_setForegroundAll(sprite, rgb);
    }

    int setForeground256(int color) {
        return Sprite_setForegroundAll256(sprite, color);
    }

    int setBackground(int x, int y, int rgb) {
        return Sprite_setBackground(sprite, x, y, rgb);
    }

    int setBackground(int rgb) {
        return Sprite_setBackgroundAll(sprite, rgb);
    }

    int setBackground256(int color) {
        return Sprite_setBackgroundAll256(sprite, color);
    }

    int rotate(double radians) {
        return Sprite_rotate(sprite, radians);
    }

    double getRotationAngle() {
        return Sprite_getRotationAngle(sprite);
    }

    int hFlip() {
        return Sprite_hFlip(sprite);
    }

    int vFlip() {
        return Sprite_vFlip(sprite);
    }

    int scale(double scale) {
        return Sprite_scale(sprite, scale);
    }

    int transpose() {
        return Sprite_transpose(sprite);
    }
};

namespace Canvas
{

/**
 * @brief Gets all of the drawn sprites.
 * @return A vector of pointers to the drawn sprites.
 */
std::vector<std::unique_ptr<Sprite>> getDrawnSprites() {
    std::vector<std::unique_ptr<Sprite>> sprites;
    CmdFX_Sprite** drawnSprites = Canvas_getDrawnSprites();
    for (int i = 0; i < Canvas_getDrawnSpritesCount(); i++) {
        sprites.push_back(std::make_unique<Sprite>(drawnSprites[i]));
    }

    return sprites;
}

/**
 * @brief Get the Sprite at a specific position.
 * @param x The x coordinate of the sprite.
 * @param y The y coordinate of the sprite.
 * @return A unique pointer to the sprite at the specified position, or nullptr
 * if no sprite is found.
 */
std::unique_ptr<Sprite> getSpriteAt(int x, int y) {
    CmdFX_Sprite* sprite = Canvas_getSpriteAt(x, y);
    if (sprite) {
        return std::make_unique<Sprite>(sprite);
    }

    return nullptr;
}

} // namespace Canvas

} // namespace CmdFX
