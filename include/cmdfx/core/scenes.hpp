/**
 * @file scenes.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the CmdFX Scenes API
 * @version 0.2.1
 * @date 2025-05-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C" {
#include "cmdfx/core/scenes.h"
}

#include "cmdfx/core/builder.hpp"
#include <memory>
#include <string>
#include <vector>

namespace CmdFX
{

/**
 * @brief A C++ wrapper around a CmdFX_Scene struct.
 *
 * This class is a wrapper around the CmdFX_Scene struct. It provides a
 * constructor and destructor for the scene, as well as methods to get and set
 * the scene's data. The destructor will free the scene's memory when the
 * object is destroyed.
 */
class Scene final {
  private:
    CmdFX_Scene* scene;

  public:
    Scene(CmdFX_Scene* scene) : scene(scene) {
    }
    Scene(int width, int height) {
        scene = Scene_create(width, height);
    }
    Scene(int width, int height, char c, char* ansi, int z) {
        scene = Scene_createFilled(width, height, c, ansi, z);
    }
    Scene(
        std::vector<std::string> data,
        std::vector<std::vector<std::string>> ansi
    ) {
        scene = Scene_createFromData(to2DArray(data), to3DArray(ansi));
    }
    Scene(char** data, char*** ansi) {
        scene = Scene_createFromData(data, ansi);
    }

    ~Scene() {
        if (scene) {
            Scene_free(scene);
        }
    }

    /**
     * @brief Get the Scene object associated with this class.
     * @return The scene object.
     */
    CmdFX_Scene* getScene() {
        return scene;
    }

    int getX() const {
        return scene->x;
    }

    int getY() const {
        return scene->y;
    }

    int getWidth() const {
        return scene->width;
    }

    int getHeight() const {
        return scene->height;
    }

    int getZ() const {
        return scene->z;
    }

    /**
     * @brief Gets the unique identifier of the scene.
     *
     * This function returns the unique identifier of the scene, which is
     * used to register and unregister the scene for drawing on the screen.
     * If the scene has not been registered, this will return `-1`.
     *
     * @return The unique identifier of the scene.
     */
    int getUniqueId() const {
        return scene->uid;
    }

    /**
     * @brief Get the data of a scene as a 1D vector of strings.
     * @return The data of the scene.
     */
    std::vector<std::string> getData() {
        std::vector<std::string> data;
        for (int i = 0; i < scene->height; i++) {
            data.push_back(std::string(scene->data[i]));
        }
        return data;
    }

    int setData(char** data) {
        return Scene_setData(scene, data);
    }

    /**
     * @brief Sets the data of a scene.
     * @param data The data to set.
     * @return The result of the operation.
     */
    int setData(std::vector<std::string> data) {
        char** text = to2DArray(data);
        return Scene_setData(scene, text);
    }

    /**
     * @brief Gets the ANSI data of a scene as a 2D vector of strings.
     * @return The ANSI data of the scene.
     */
    std::vector<std::vector<std::string>> getAnsi() {
        std::vector<std::vector<std::string>> ansi;
        for (int i = 0; i < scene->height; i++) {
            std::vector<std::string> row;
            for (int j = 0; j < scene->width; j++) {
                row.push_back(std::string(scene->ansiData[i][j]));
            }
            ansi.push_back(row);
        }
        return ansi;
    }

    int setAnsi(char*** ansi) {
        return Scene_setAnsiData(scene, ansi);
    }

    /**
     * @brief Sets the ANSI data of a scene.
     * @param ansi The ANSI data to set.
     * @return The result of the operation.
     */
    int setAnsi(std::vector<std::vector<std::string>> ansi) {
        char*** ansiData = to3DArray(ansi);
        return Scene_setAnsiData(scene, ansiData);
    }

    int appendAnsi(char*** ansi) {
        return Scene_appendAnsiData(scene, ansi);
    }

    /**
     * @brief Appends the ANSI data to the current ANSI data of the scene.
     * @param ansi The ANSI data to append.
     * @return The result of the operation.
     */
    int appendAnsi(std::vector<std::vector<std::string>> ansi) {
        char*** ansiData = to3DArray(ansi);
        return Scene_appendAnsiData(scene, ansiData);
    }

    int setForeground(int x, int y, int width, int height, int rgb) {
        return Scene_setForeground(scene, x, y, width, height, rgb);
    }

    int setForegroundAll(int rgb) {
        return Scene_setForegroundAll(scene, rgb);
    }

    int setBackground(int x, int y, int width, int height, int rgb) {
        return Scene_setBackground(scene, x, y, width, height, rgb);
    }

    int setBackgroundAll(int rgb) {
        return Scene_setBackgroundAll(scene, rgb);
    }

    int clear() {
        return Scene_clear(scene);
    }

    int draw(int x, int y) {
        return Scene_draw(scene, x, y);
    }

    int drawPortion(int x, int y, int sx, int sy, int width, int height) {
        return Scene_drawPortion(scene, x, y, sx, sy, width, height);
    }

    bool isOnTop() const {
        return Scene_isOnTop(scene);
    }

    bool isOnTopAt(int x, int y) const {
        return Scene_isOnTopAt(scene, x, y);
    }

    bool isOnBottom() const {
        return Scene_isOnBottom(scene);
    }

    bool isOnBottomAt(int x, int y) const {
        return Scene_isOnBottomAt(scene, x, y);
    }

    int remove() {
        return Scene_remove(scene);
    }

    bool isEmpty() const {
        return Scene_isEmpty(scene);
    }

    int switchTo(int x, int y) {
        return Scene_switchTo(scene, x, y);
    }

    static std::unique_ptr<Scene> getSceneAt(int x, int y) {
        CmdFX_Scene* foundScene = Scene_getSceneAt(x, y);
        if (foundScene == nullptr) return nullptr;
        return std::make_unique<Scene>(foundScene);
    }

    /**
     * @brief Registers the scene to be drawn on the screen.
     * @return `scene->uid` if the scene was registered successfully, or `-1` if
     * an error occurred.
     */
    int registerScene() {
        return Scene_register(scene);
    }

    /**
     * @brief Unregisters the scene from being drawn on the screen.
     * @return The unique identifier of the scene before it was unregistered, or
     * `-1` if an error occurred.
     */
    int unregisterScene() {
        return Scene_unregister(scene);
    }

    static int drawRegistered(int uid, int x, int y) {
        return Scene_drawRegistered(uid, x, y);
    }

    static int switchToRegistered(int uid, int x, int y) {
        return Scene_switchToRegistered(uid, x, y);
    }

    static int scroll(int uid, int dx, int dy) {
        return Scene_scroll(uid, dx, dy);
    }

    /**
     * @brief Scrolls the scene by the specified amount.
     *
     * This function scrolls the scene by the specified amount in the x and y
     * directions. The amount is specified by the `dx` and `dy` parameters,
     * which represent the amount to scroll in the x and y directions,
     * respectively. This requires the scene to be registered with the Scene
     * Engine.
     *
     * @param dx The amount to scroll in the x direction.
     * @param dy The amount to scroll in the y direction.
     * @return `0` if the scene was scrolled successfully, or `-1` if an error
     * occurred.
     */
    int scroll(int dx, int dy) {
        if (scene->uid < 0) return -1;
        return Scene_scroll(scene->uid, dx, dy);
    }
};

namespace Canvas
{

/**
 * @brief Gets all of the drawn scenes.
 * @return A vector of pointers to the drawn scenes.
 */
std::vector<std::unique_ptr<Scene>> getDrawnScenes() {
    std::vector<std::unique_ptr<Scene>> scenes;
    CmdFX_Scene** drawnScenes = Canvas_getDrawnScenes();
    for (int i = 0; i < Canvas_getDrawnScenesCount(); i++) {
        scenes.push_back(std::make_unique<Scene>(drawnScenes[i]));
    }
    return scenes;
}

/**
 * @brief Gets the number of scenes that are drawn on the screen.
 * @return The number of scenes that are drawn on the screen.
 */
int getDrawnScenesCount() {
    return Canvas_getDrawnScenesCount();
}

/**
 * @brief Gets the registered scenes.
 * @return A vector of pointers to the registered scenes.
 */
std::vector<std::unique_ptr<Scene>> getRegisteredScenes() {
    std::vector<std::unique_ptr<Scene>> scenes;
    CmdFX_Scene** registeredScenes = Scene_getRegisteredScenes();
    for (int i = 0; i < Scene_getRegisteredScenesCount(); i++) {
        scenes.push_back(std::make_unique<Scene>(registeredScenes[i]));
    }
    return scenes;
}

/**
 * @brief Gets the number of registered scenes.
 * @return The number of registered scenes.
 */
int getRegisteredScenesCount() {
    return Scene_getRegisteredScenesCount();
}

/**
 * @brief Gets a registered scene by its unique identifier.
 * @param uid The unique identifier of the scene to get.
 * @return A unique pointer to the registered scene, or nullptr if the scene was
 * not found.
 */
std::unique_ptr<Scene> getRegisteredScene(int uid) {
    CmdFX_Scene* scene = Scene_getRegisteredScene(uid);
    if (scene) {
        return std::make_unique<Scene>(scene);
    }

    return nullptr;
}

} // namespace Canvas

/**
 * @brief Scene Engine functions
 */
namespace SceneEngine
{

/**
 * @brief Ticks the Scene Engine.
 *
 * This function ticks the Scene Engine, updating all registered scenes
 * and redrawing them if necessary. This should be called once per frame.
 */
void tick() {
    tickCmdFXSceneEngine();
}

/**
 * @brief Begins the Scene Engine.
 *
 * This function starts the Scene Engine, enabling automatic scene
 * management and rendering. This should be called before registering
 * any scenes.
 *
 * @return 0 if successful, -1 if an error occurred.
 */
int begin() {
    return beginCmdFXSceneEngine();
}

/**
 * @brief Ends the Scene Engine.
 *
 * This function stops the Scene Engine and cleans up all resources.
 * This should be called when scene management is no longer needed.
 *
 * @return 0 if successful, -1 if an error occurred.
 */
int end() {
    return endCmdFXSceneEngine();
}

} // namespace SceneEngine

} // namespace CmdFX
