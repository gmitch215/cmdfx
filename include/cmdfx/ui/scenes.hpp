/**
 * @file scenes.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for CmdFX UI Scenes
 * @version 0.2.1
 * @date 2025-05-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C" {
#include "cmdfx/core/scenes.h"
#include "cmdfx/ui/scenes.h"
#include <stdlib.h>
}

#include "cmdfx/core/scenes.hpp"
#include "cmdfx/ui/button.hpp"
#include <memory>
#include <vector>

namespace CmdFX
{

class SceneButtons final {
  private:
    int sceneUid;

  public:
    SceneButtons(int uid) : sceneUid(uid) {
    }
    SceneButtons(const Scene& scene) : sceneUid(scene.getUniqueId()) {
    }
    ~SceneButtons() {
        Scene_removeAllButtons(sceneUid);
    }

    /**
     * @brief Gets the buttons in the scene.
     * @return A vector of unique_ptr<Button> containing all buttons in the
     * scene.
     */
    std::vector<std::unique_ptr<Button>> getButtons() {
        CmdFX_Button** buttons = Scene_getButtons(sceneUid);
        int count = Scene_getButtonsCount(sceneUid);

        std::vector<std::unique_ptr<Button>> buttonList;
        buttonList.reserve(count);

        for (int i = 0; i < count; i++) {
            if (buttons[i] != nullptr) {
                buttonList.emplace_back(std::make_unique<Button>(buttons[i]));
            }
        }

        return buttonList;
    }

    std::vector<int> getButtonCoordinates(int index, Button& button) {
        int* coords = Scene_getButtonCoordinates(sceneUid, button.getButton());
        if (coords == nullptr) return {-1, -1};

        std::vector<int> coordinates = {coords[0], coords[1]};
        return coordinates;
    }

    int addButton(Button& button, int x, int y) {
        return Scene_addButton(sceneUid, button.getButton(), x, y);
    }

    int removeButton(Button& button) {
        return Scene_removeButton(sceneUid, button.getButton());
    }

    int removeButtonAt(int x, int y) {
        return Scene_removeButtonAt(sceneUid, x, y);
    }

    int removeAllButtons() {
        return Scene_removeAllButtons(sceneUid);
    }
};

} // namespace CmdFX