/**
 * @file canvas.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Canvas C++ Extension Header
 * @version 0.2.1
 * @date 2025-05-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include <string>
#include "cmdfx/core/canvas.h"

namespace CmdFX {
    /**
     * @brief C++ wrapper for the CmdFX canvas.
     * 
     */
    class Canvas {
        public:
            static int getWidth() { return Canvas_getWidth(); }
            static int getHeight() { return Canvas_getHeight(); }
            static void clearScreen() { Canvas_clearScreen(); }
            static void setCursor(int x, int y) { Canvas_setCursor(x, y); }
            static int getCursorX() { return Canvas_getCursorX(); }
            static int getCursorY() { return Canvas_getCursorY(); }
            static void hideCursor() { Canvas_hideCursor(); }
            static void showCursor() { Canvas_showCursor(); }
            static int isCursorVisible() { return Canvas_isCursorVisible(); }
            static void setChar(int x, int y, char c) { Canvas_setChar(x, y, c); }
            static void setAnsiCurrent(const char* ansi) { Canvas_setAnsiCurrent(ansi); }
            static void setAnsiCurrent(const std::string& ansi) { Canvas_setAnsiCurrent(ansi.c_str()); }
            static void setAnsi(int x, int y, const char* ansi) { Canvas_setAnsi(x, y, ansi); }
            static void setAnsi(int x, int y, const std::string& ansi) { Canvas_setAnsi(x, y, ansi.c_str()); }
            static void rect(int x, int y, int width, int height, char c) { Canvas_rect(x, y, width, height, c); }
            static void fillRect(int x, int y, int width, int height, char c) { Canvas_fillRect(x, y, width, height, c); }
            static void circle(int x, int y, int radius, char c) { Canvas_circle(x, y, radius, c); }
            static void fillCircle(int x, int y, int radius, char c) { Canvas_fillCircle(x, y, radius, c); }
            static void ellipse(int x, int y, int xradius, int yradius, char c) { Canvas_ellipse(x, y, xradius, yradius, c); }
            static void fillEllipse(int x, int y, int xradius, int yradius, char c) { Canvas_fillEllipse(x, y, xradius, yradius, c); }
            static void arc(int x, int y, int rx, int ry, double xrot, int arcflag, int sweepflag, int dx, int dy, char c) { Canvas_arc(x, y, rx, ry, xrot, arcflag, sweepflag, dx, dy, c); }
            static void drawText(int x, int y, char* text) { Canvas_drawText(x, y, text); }
            static void drawText(int x, int y, const std::string& text) { Canvas_drawText(x, y, const_cast<char*>(text.c_str())); }
            static void drawAscii(int x, int y, char ascii[8][5]) { Canvas_drawAscii(x, y, ascii); }
            static void drawAsciiText(int x, int y, char c, const char* text) { Canvas_drawAsciiText(x, y, c, text); }
    };
}