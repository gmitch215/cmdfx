/**
 * @file builder.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for the Builder API
 * @version 0.2.1
 * @date 2025-05-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C"
{
#include "cmdfx/core/builder.h"
}

#include <string>
#include <vector>

namespace CmdFX
{
int getWidth(char** array) {
    return getCharArrayWidth(array);
}
int getWidth(char*** array) {
    return getStringArrayWidth(array);
}
int getHeight(char** array) {
    return getCharArrayHeight(array);
}
int getHeight(char*** array) {
    return getStringArrayHeight(array);
}
char** createCopy(char** array) {
    return createCharArrayCopy(array);
}
char*** createCopy(char*** array) {
    return createStringArrayCopy(array);
}
int isSameSize(char** array1, char** array2) {
    return areCharArraysSameSize(array1, array2);
}
int isSameSize(char*** array1, char*** array2) {
    return areStringArraysSameSize(array1, array2);
}
int compare(char** array1, char** array2) {
    return compareCharArrays(array1, array2);
}
int compare(char*** array1, char*** array2) {
    return compareStringArrays(array1, array2);
}
int print(char** array) {
    return printCharArray(array);
}
int print(char*** array) {
    return printStringArray(array);
}

/**
 * @brief Converts a 1D vector of strings to a 2D array of characters.
 * @param string The vector of strings to convert.
 * @return char** The 2D array of characters.
 */
char** to2DArray(std::vector<std::string> string) {
    char** array = Builder::createCharArray(string.size(), string[0].size());
    for (int i = 0; i < string.size(); i++) {
        for (int j = 0; j < string[i].size(); j++) {
            array[i][j] = string[i][j];
        }
    }

    return array;
}

/**
 * @brief Converts a 2D vector of strings to a 3D array of characters.
 * @param string The vector of strings to convert.
 * @return char*** The 3D array of characters.
 */
char*** to3DArray(std::vector<std::vector<std::string>> string) {
    char*** array = Builder::createStringArray(string.size(), string[0].size());
    for (int i = 0; i < string.size(); i++) {
        for (int j = 0; j < string[i].size(); j++) {
            array[i][j] = const_cast<char*>(string[i][j].c_str());
        }
    }

    return array;
}

/**
 * @brief C++ wrapper for the CmdFX Builder API.
 *
 */
namespace Builder
{
char** createCharArray(int width, int height) {
    return Char2DBuilder_create(width, height);
}
char** createCharArrayFilled(int width, int height, char c) {
    return Char2DBuilder_createFilled(width, height, c);
}
char*** createStringArray(int width, int height) {
    return String2DBuilder_create(width, height);
}
char*** createStringArrayFilled(int width, int height, const std::string& c) {
    return String2DBuilder_createFilled(
        width, height, const_cast<char*>(c.c_str())
    );
}
char*** createStringArrayFilled(int width, int height, char* c) {
    return String2DBuilder_createFilled(width, height, c);
}
int set(char** array, int x, int y, char c) {
    return Char2DBuilder_setChar(array, x, y, c);
}
int set(char*** array, int x, int y, const std::string& c) {
    return String2DBuilder_setString(array, x, y, const_cast<char*>(c.c_str()));
}
int set(char*** array, int x, int y, char* c) {
    return String2DBuilder_setString(array, x, y, c);
}
int hLine(char** array, int x, int y, int length, char c) {
    return Char2DBuilder_hLine(array, x, y, length, c);
}
int hLine(char*** array, int x, int y, int length, const std::string& c) {
    return String2DBuilder_hLine(
        array, x, y, length, const_cast<char*>(c.c_str())
    );
}
int hLine(char*** array, int x, int y, int length, char* c) {
    return String2DBuilder_hLine(array, x, y, length, c);
}
int vLine(char** array, int x, int y, int length, char c) {
    return Char2DBuilder_vLine(array, x, y, length, c);
}
int vLine(char*** array, int x, int y, int length, const std::string& c) {
    return String2DBuilder_vLine(
        array, x, y, length, const_cast<char*>(c.c_str())
    );
}
int vLine(char*** array, int x, int y, int length, char* c) {
    return String2DBuilder_vLine(array, x, y, length, c);
}
int rect(char** array, int x, int y, int width, int height, char c) {
    return Char2DBuilder_rect(array, x, y, width, height, c);
}
int rect(
    char*** array, int x, int y, int width, int height, const std::string& c
) {
    return String2DBuilder_rect(
        array, x, y, width, height, const_cast<char*>(c.c_str())
    );
}
int rect(char*** array, int x, int y, int width, int height, char* c) {
    return String2DBuilder_rect(array, x, y, width, height, c);
}
int fillRect(char** array, int x, int y, int width, int height, char c) {
    return Char2DBuilder_fillRect(array, x, y, width, height, c);
}
int fillRect(
    char*** array, int x, int y, int width, int height, const std::string& c
) {
    return String2DBuilder_fillRect(
        array, x, y, width, height, const_cast<char*>(c.c_str())
    );
}
int fillRect(char*** array, int x, int y, int width, int height, char* c) {
    return String2DBuilder_fillRect(array, x, y, width, height, c);
}
int circle(char** array, int x, int y, int radius, char c) {
    return Char2DBuilder_circle(array, x, y, radius, c);
}
int circle(char*** array, int x, int y, int radius, const std::string& c) {
    return String2DBuilder_circle(
        array, x, y, radius, const_cast<char*>(c.c_str())
    );
}
int circle(char*** array, int x, int y, int radius, char* c) {
    return String2DBuilder_circle(array, x, y, radius, c);
}
int fillCircle(char** array, int x, int y, int radius, char c) {
    return Char2DBuilder_fillCircle(array, x, y, radius, c);
}
int fillCircle(char*** array, int x, int y, int radius, const std::string& c) {
    return String2DBuilder_fillCircle(
        array, x, y, radius, const_cast<char*>(c.c_str())
    );
}
int fillCircle(char*** array, int x, int y, int radius, char* c) {
    return String2DBuilder_fillCircle(array, x, y, radius, c);
}
int ellipse(char** array, int x, int y, int xradius, int yradius, char c) {
    return Char2DBuilder_ellipse(array, x, y, xradius, yradius, c);
}
int ellipse(
    char*** array, int x, int y, int xradius, int yradius, const std::string& c
) {
    return String2DBuilder_ellipse(
        array, x, y, xradius, yradius, const_cast<char*>(c.c_str())
    );
}
int ellipse(char*** array, int x, int y, int xradius, int yradius, char* c) {
    return String2DBuilder_ellipse(array, x, y, xradius, yradius, c);
}
int fillEllipse(char** array, int x, int y, int xradius, int yradius, char c) {
    return Char2DBuilder_fillEllipse(array, x, y, xradius, yradius, c);
}
int fillEllipse(
    char*** array, int x, int y, int xradius, int yradius, const std::string& c
) {
    return String2DBuilder_fillEllipse(
        array, x, y, xradius, yradius, const_cast<char*>(c.c_str())
    );
}
int fillEllipse(
    char*** array, int x, int y, int xradius, int yradius, char* c
) {
    return String2DBuilder_fillEllipse(array, x, y, xradius, yradius, c);
}
int fill(char** array, char c) {
    return Char2DBuilder_fill(array, c);
}
int fill(char*** array, const std::string& c) {
    return String2DBuilder_fill(array, const_cast<char*>(c.c_str()));
}
int fill(char*** array, char* c) {
    return String2DBuilder_fill(array, c);
}
int line(char** array, int x1, int y1, int x2, int y2, char c) {
    return Char2DBuilder_line(array, x1, y1, x2, y2, c);
}
int line(char*** array, int x1, int y1, int x2, int y2, const std::string& c) {
    return String2DBuilder_line(
        array, x1, y1, x2, y2, const_cast<char*>(c.c_str())
    );
}
int line(char*** array, int x1, int y1, int x2, int y2, char* c) {
    return String2DBuilder_line(array, x1, y1, x2, y2, c);
}
int polygon(char** array, int x, int y, int sides, int points, char c) {
    return Char2DBuilder_polygon(array, x, y, sides, points, c);
}
int polygon(
    char*** array, int x, int y, int sides, int points, const std::string& c
) {
    return String2DBuilder_polygon(
        array, x, y, sides, points, const_cast<char*>(c.c_str())
    );
}
int polygon(char*** array, int x, int y, int sides, int points, char* c) {
    return String2DBuilder_polygon(array, x, y, sides, points, c);
}
int fillPolygon(char** array, int x, int y, int sides, int points, char c) {
    return Char2DBuilder_fillPolygon(array, x, y, sides, points, c);
}
int fillPolygon(
    char*** array, int x, int y, int sides, int points, const std::string& c
) {
    return String2DBuilder_fillPolygon(
        array, x, y, sides, points, const_cast<char*>(c.c_str())
    );
}
int fillPolygon(char*** array, int x, int y, int sides, int points, char* c) {
    return String2DBuilder_fillPolygon(array, x, y, sides, points, c);
}
int text(char** array, int x, int y, const std::string& text) {
    return Char2DBuilder_text(array, x, y, const_cast<char*>(text.c_str()));
}

int resize(char** array, int width, int height) {
    return Char2DBuilder_resize(array, width, height);
}
int resizeWithPadding(char** array, int width, int height, char padding) {
    return Char2DBuilder_resizeWithPadding(array, width, height, padding);
}
int center(char** array) {
    return Char2DBuilder_center(array);
}
int resizeAndCenter(char** array, int width, int height) {
    return Char2DBuilder_resizeAndCenter(array, width, height);
}
int rotate(char** array, double radians) {
    return Char2DBuilder_rotate(array, radians);
}
int rotate(char*** array, double radians) {
    return String2DBuilder_rotate(array, radians);
}
double getRotationAngle(char** array) {
    return Char2DBuilder_getRotationAngle(array);
}
int hFlip(char** array) {
    return Char2DBuilder_hFlip(array);
}
int hFlip(char*** array) {
    return String2DBuilder_hFlip(array);
}
int vFlip(char** array) {
    return Char2DBuilder_vFlip(array);
}
int vFlip(char*** array) {
    return String2DBuilder_vFlip(array);
}
char** transpose(char** array) {
    return Char2DBuilder_transpose(array);
}
char*** transpose(char*** array) {
    return String2DBuilder_transpose(array);
}
int replaceAll(char** array, char find, char replace) {
    return Char2DBuilder_replaceAll(array, find, replace);
}
int replaceAll(
    char*** array, const std::string& find, const std::string& replace
) {
    return String2DBuilder_replaceAll(
        array, const_cast<char*>(find.c_str()),
        const_cast<char*>(replace.c_str())
    );
}
int replaceAll(char*** array, char* find, char* replace) {
    return String2DBuilder_replaceAll(array, find, replace);
}
char** scale(char** array, double scale) {
    return Char2DBuilder_scale(array, scale);
}

int gradient(
    char** array, int x, int y, int width, int height, char start, char end,
    enum CmdFX_GradientDirection direction
) {
    return Char2DBuilder_gradient(
        array, x, y, width, height, start, end, direction
    );
}

int gradientForeground(
    char*** array, int x, int y, int width, int height, int start, int end,
    enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_gradientForeground(
        array, x, y, width, height, start, end, direction
    );
}
int gradientForegroundFull(
    char*** array, int start, int end, enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_gradientForegroundFull(array, start, end, direction);
}
int gradientBackground(
    char*** array, int x, int y, int width, int height, int start, int end,
    enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_gradientBackground(
        array, x, y, width, height, start, end, direction
    );
}
int gradientBackgroundFull(
    char*** array, int start, int end, enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_gradientBackgroundFull(array, start, end, direction);
}
int multiGradientForeground(
    char*** array, int x, int y, int width, int height, int numColors,
    int* colors, enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientForeground(
        array, x, y, width, height, numColors, colors, direction
    );
}
int multiGradientForegroundFull(
    char*** array, int numColors, int* colors,
    enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientForegroundFull(
        array, numColors, colors, direction
    );
}
int multiGradientBackground(
    char*** array, int x, int y, int width, int height, int numColors,
    int* colors, enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientBackground(
        array, x, y, width, height, numColors, colors, direction
    );
}
int multiGradientBackgroundFull(
    char*** array, int numColors, int* colors,
    enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientBackgroundFull(
        array, numColors, colors, direction
    );
}
int multiGradientsForeground(
    char*** array, int x, int y, int width, int height, int numColors,
    int* colors, double* percentages, enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientsForeground(
        array, x, y, width, height, numColors, colors, percentages, direction
    );
}
int multiGradientsForegroundFull(
    char*** array, int numColors, int* colors, double* percentages,
    enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientsForegroundFull(
        array, numColors, colors, percentages, direction
    );
}
int multiGradientsBackground(
    char*** array, int x, int y, int width, int height, int numColors,
    int* colors, double* percentages, enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientsBackground(
        array, x, y, width, height, numColors, colors, percentages, direction
    );
}
int multiGradientsBackgroundFull(
    char*** array, int numColors, int* colors, double* percentages,
    enum CmdFX_GradientDirection direction
) {
    return String2DBuilder_multiGradientsBackgroundFull(
        array, numColors, colors, percentages, direction
    );
}
}; // namespace Builder
} // namespace CmdFX