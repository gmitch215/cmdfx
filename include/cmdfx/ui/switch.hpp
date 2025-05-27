/**
 * @file switch.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for CmdFX Switch Buttons
 * @version 0.2.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

extern "C" {
    #include "cmdfx/ui/switch.h"
}

#include "cmdfx/core/builder.hpp"
#include "cmdfx/ui/button.hpp"

namespace CmdFX {

    /**
     * @brief A C++ wrapper around the CmdFX_Button struct for switch buttons.
     * 
     * This class represents a switch button in the CmdFX UI system. It provides methods
     * to interact with the switch, such as toggling its state and checking if it is on or off.
     */
    class Switch : public Button {
        public:
            Switch(CmdFX_Button* btn) : Button(btn) {}
            Switch(Sprite& sprite, CmdFX_ButtonCallback callback, bool state = false) 
                : Button(Button_createSwitch(sprite.getSprite(), callback, state)) {}
            Switch(std::vector<std::string>& on, std::vector<std::string>& off, std::vector<std::vector<std::string>>& ansiOn, std::vector<std::vector<std::string>>& ansiOff, CmdFX_ButtonCallback callback, bool state = false)
                : Button(Button_createSwitchWith(to2DArray(on), to2DArray(off), to3DArray(ansiOn), to3DArray(ansiOff), callback, state)) {}

            bool isOn() const {
                return Switch_getState(getButton());
            }

            void setOn(bool state) {
                Switch_setState(getButton(), state);
            }

            void toggle() {
                if (isOn()) {
                    setOn(false);
                } else {
                    setOn(true);
                }   
            }
    };

}