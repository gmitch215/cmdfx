/**
 * @file screen.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the CmdFX Screen API
 * @version 0.2.1
 * @date 2025-05-20
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <string>

extern "C"
{
#include "cmdfx/core/screen.h"
}

namespace CmdFX
{
/**
 * @brief C++ wrapper for the CmdFX Screen API.
 */
namespace Screen
{
void getScreenSize(int* width, int* height) {
    Screen_getSize(width, height);
}
void getMousePos(int* x, int* y) {
    Screen_getMousePos(x, y);
}

int isEchoEnabled() {
    return Screen_isEchoEnabled();
}
void setEchoEnabled(int echoEnabled) {
    Screen_setEchoEnabled(echoEnabled);
}
int isLineBuffered() {
    return Screen_isLineBuffered();
}
void setLineBuffered(int lineBuffered) {
    Screen_setLineBuffered(lineBuffered);
}

bool isInTerminal() {
    return Screen_isInTerminal();
}
void ensureInTerminal() {
    Screen_ensureInTerminal();
}
}; // namespace Screen

/**
 * @brief C++ wrapper for the CmdFX Window API.
 */
namespace Window
{
const char* getTitle() {
    return Window_getTitle();
}
void setTitle(const char* title) {
    Window_setTitle(title);
}
void setTitle(const std::string& title) {
    Window_setTitle(title.c_str());
}
void getSize(int* width, int* height) {
    Window_getSize(width, height);
}
void setSize(int width, int height) {
    Window_setSize(width, height);
}
}; // namespace Window
} // namespace CmdFX