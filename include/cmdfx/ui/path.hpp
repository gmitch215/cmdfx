/**
 * @file path.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for CmdFX Path Drawing
 * @version 0.2.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once 

extern "C" {
    #include "cmdfx/ui/path.h"
}

#include <string>

namespace CmdFX {

    namespace Canvas {
        /**
         * @brief Draws a path on the canvas using the specified path string.
         * 
         * This function interprets the path string as a series of SVG-like commands
         * and draws the corresponding shapes on the canvas. The path string can contain
         * commands for moving to a point, drawing lines, rectangles, ellipses, and
         * curves, as well as closing the path.
         * 
         * @param path The path string to draw
         * @param ch The character to draw
         */
        inline void path(const std::string path, char ch) {
            Canvas_path(path.c_str(), ch);
        }

    }

}