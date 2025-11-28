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

extern "C" {
#include "cmdfx/core/canvas.h"
}

namespace CmdFX
{
/**
 * @brief C++ wrapper for the CmdFX canvas.
 */
namespace Canvas
{
int getWidth() {
    return Canvas_getWidth();
}
int getHeight() {
    return Canvas_getHeight();
}
void clearScreen() {
    Canvas_clearScreen();
}
void setCursor(int x, int y) {
    Canvas_setCursor(x, y);
}
int getCursorX() {
    return Canvas_getCursorX();
}
int getCursorY() {
    return Canvas_getCursorY();
}
void hideCursor() {
    Canvas_hideCursor();
}
void showCursor() {
    Canvas_showCursor();
}
int isCursorVisible() {
    return Canvas_isCursorVisible();
}
void setChar(int x, int y, char c) {
    Canvas_setChar(x, y, c);
}
void setAnsiCurrent(const char* ansi) {
    Canvas_setAnsiCurrent(ansi);
}
void setAnsiCurrent(const std::string& ansi) {
    Canvas_setAnsiCurrent(ansi.c_str());
}
void setAnsi(int x, int y, const char* ansi) {
    Canvas_setAnsi(x, y, ansi);
}
void setAnsi(int x, int y, const std::string& ansi) {
    Canvas_setAnsi(x, y, ansi.c_str());
}
void rect(int x, int y, int width, int height, char c) {
    Canvas_rect(x, y, width, height, c);
}
void fillRect(int x, int y, int width, int height, char c) {
    Canvas_fillRect(x, y, width, height, c);
}
void circle(int x, int y, int radius, char c) {
    Canvas_circle(x, y, radius, c);
}
void fillCircle(int x, int y, int radius, char c) {
    Canvas_fillCircle(x, y, radius, c);
}
void ellipse(int x, int y, int xradius, int yradius, char c) {
    Canvas_ellipse(x, y, xradius, yradius, c);
}
void fillEllipse(int x, int y, int xradius, int yradius, char c) {
    Canvas_fillEllipse(x, y, xradius, yradius, c);
}
void arc(
    int x, int y, int rx, int ry, double xrot, int arcflag, int sweepflag,
    int dx, int dy, char c
) {
    Canvas_arc(x, y, rx, ry, xrot, arcflag, sweepflag, dx, dy, c);
}
void drawText(int x, int y, char* text) {
    Canvas_drawText(x, y, text);
}
void drawText(int x, int y, const std::string& text) {
    Canvas_drawText(x, y, const_cast<char*>(text.c_str()));
}
void drawAscii(int x, int y, char ascii[8][5]) {
    Canvas_drawAscii(x, y, ascii);
}
void drawAsciiText(int x, int y, char c, const char* text) {
    Canvas_drawAsciiText(x, y, c, text);
}
void drawAsciiText(int x, int y, char c, const std::string& text) {
    Canvas_drawAsciiText(x, y, c, text.c_str());
}
void resetFormat() {
    Canvas_resetFormat();
}
void setForeground(int rgb) {
    Canvas_setForeground(rgb);
}
void setBackground(int rgb) {
    Canvas_setBackground(rgb);
}
void setColor8(int color) {
    Canvas_setColor8(color);
}
void setForeground256(int color) {
    Canvas_setForeground256(color);
}
void setBackground256(int color) {
    Canvas_setBackground256(color);
}
void enableBold() {
    Canvas_enableBold();
}
void disableBold() {
    Canvas_disableBold();
}
void enableDim() {
    Canvas_enableDim();
}
void disableDim() {
    Canvas_disableDim();
}
void enableItalic() {
    Canvas_enableItalic();
}
void disableItalic() {
    Canvas_disableItalic();
}
void enableUnderline() {
    Canvas_enableUnderline();
}
void disableUnderline() {
    Canvas_disableUnderline();
}
void enableBlink() {
    Canvas_enableBlink();
}
void disableBlink() {
    Canvas_disableBlink();
}
void enableInvert() {
    Canvas_enableInvert();
}
void disableInvert() {
    Canvas_disableInvert();
}
void enableHidden() {
    Canvas_enableHidden();
}
void disableHidden() {
    Canvas_disableHidden();
}
void enableStrikethrough() {
    Canvas_enableStrikethrough();
}
void disableStrikethrough() {
    Canvas_disableStrikethrough();
}
void hLine(int x, int y, int width, char c) {
    Canvas_hLine(x, y, width, c);
}
void vLine(int x, int y, int height, char c) {
    Canvas_vLine(x, y, height, c);
}
void line(int x1, int y1, int x2, int y2, char c) {
    Canvas_line(x1, y1, x2, y2, c);
}
void polygon(int x, int y, int sides, int radius, char c) {
    Canvas_polygon(x, y, sides, radius, c);
}
void fillPolygon(int x, int y, int sides, int radius, char c) {
    Canvas_fillPolygon(x, y, sides, radius, c);
}
void quad(int x, int y, int x1, int y1, int x2, int y2, char c) {
    Canvas_quad(x, y, x1, y1, x2, y2, c);
}
void cubic(
    int x, int y, int x1, int y1, int x2, int y2, int x3, int y3, char c
) {
    Canvas_cubic(x, y, x1, y1, x2, y2, x3, y3, c);
}
}; // namespace Canvas
} // namespace CmdFX
