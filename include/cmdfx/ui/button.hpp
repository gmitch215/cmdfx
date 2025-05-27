/**
 * @file button.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for CmdFX Button UI
 * @version 0.2.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

extern "C" {
    #include <stdlib.h>
    #include "cmdfx/core/events.h"
    #include "cmdfx/ui/button.h"
}

#include "cmdfx/core/sprites.hpp"
#include "cmdfx/core/builder.hpp"
#include <vector>
#include <memory>

namespace CmdFX {

    /**
     * @brief A C++ wrapper around the CmdFX_Button struct.
     * 
     * This class represents a button in the CmdFX UI system.
     * It provides methods to interact with the button, such as setting data,
     * showing, hiding, and checking if the button is hidden. The button
     * will be freed automatically when the Button object is destroyed.
     */
    class Button {
        private:
            CmdFX_Button* button;
        
        public:
            Button(CmdFX_Button* btn) : button(btn) {}
            Button(Sprite& sprite, CmdFX_ButtonCallback callback) {
                button = Button_create(sprite.getSprite(), callback);
            }
            Button(int width, int height, char c, char* ansi, int z, CmdFX_ButtonCallback callback) {
                button = Button_createFilled(width, height, c, ansi, z, callback);
            }
            ~Button() {
                if (button) { Button_free(button); }
            }

            CmdFX_Button* getButton() const {
                return button;
            }

            /**
             * @brief Gets the value of the button's sprite.
             * @return A pointer to the CmdFX_Sprite associated with this button.
             */
            CmdFX_Sprite* getSprite() const {
                return button->sprite;
            }

            int draw(int x, int y) {
                return Button_draw(x, y, button);
            }

            int remove() {
                return Button_remove(button);
            }

            bool isHidden() const {
                return Button_isHidden(button) != 0;
            }

            int hide() {
                return Button_hide(button);
            }

            int show() {
                return Button_show(button);
            }

            int setData(char** data, char*** ansi) {
                return Button_setData(button, data, ansi);
            }

            /**
             * @brief Sets the data for the button using vectors.
             * @param data A 1D vector of strings representing the data to set for the button.
             * @param ansi A 2D vector of strings representing the ANSI escape codes to apply to the data.
             * @return 0 if successful, else -1
             */
            int setData(std::vector<std::string> data, std::vector<std::vector<std::string>> ansi) {
                return Button_setData(button, to2DArray(data), to3DArray(ansi));
            }

            int moveTo(int x, int y) {
                return Button_moveTo(button, x, y);
            }

            int moveBy(int dx, int dy) {
                return Button_moveBy(button, dx, dy);
            }
    };

    namespace Canvas {
        /**
         * @brief Gets all registered buttons in the UI manager as a vector of unique_ptr<Button>.
         * @return A vector of unique_ptr<Button> containing all registered buttons.
         */
        std::vector<std::unique_ptr<Button>> getRegisteredButtons() {
            CmdFX_Button** buttons = Canvas_getRegisteredButtons();
            int count = Canvas_getRegisteredButtonsCount();

            std::vector<std::unique_ptr<Button>> buttonList;
            buttonList.reserve(count);

            for (int i = 0; i < count; i++) {
                if (buttons[i] != nullptr) {
                    buttonList.emplace_back(std::make_unique<Button>(buttons[i]));
                }
            }

            return buttonList;
        }

        int getRegisteredButtonsCount() {
            return Canvas_getRegisteredButtonsCount();
        }

        std::vector<std::unique_ptr<Button>> getAllButtonsAt(int x, int y) {
            CmdFX_Button** buttons = Canvas_getAllButtonsAt(x, y);
            if (buttons == nullptr) return {};

            std::vector<std::unique_ptr<Button>> buttonList;
            for (int i = 0; buttons[i] != nullptr; i++) {
                buttonList.emplace_back(std::make_unique<Button>(buttons[i]));
            }

            free(buttons);
            return buttonList;
        }

        std::unique_ptr<Button> getButtonAt(int x, int y) {
            CmdFX_Button* button = Canvas_getButtonAt(x, y);
            if (button == nullptr) return nullptr;

            return std::make_unique<Button>(button);
        }
    }

}