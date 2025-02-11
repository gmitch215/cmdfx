/**
 * @file util.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Cross-Platform Utilities for CmdFX.
 * @version 0.1.0
 * @date 2025-01-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Global Declarations

/**
 * @brief Gets the tick speed of cmdfx.
 * 
 * The tick speed controls the rate of which ticking happens, such
 * as the physics engine. The default value is 12.
 * 
 * @return The cmdfx Tick Speed
 */
int CmdFX_getTickSpeed();

/**
 * @brief Sets the tick speed of cmdfx.
 * 
 * The tick speed controls the rate of which ticking happens, such
 * as the physics engine. The default value is 12.
 * 
 * @param tickspeed The new tickspeed to set
 * @return 0 if successful, -1 if an error happened
 */
int CmdFX_setTickSpeed(int tickspeed);

// Time

/**
 * @brief Gets the current time in milliseconds.
 * @return The current time in milliseconds.
 */
unsigned long currentTimeMillis();

/**
 * @brief Gets the current time in nanoseconds.
 * @return The current time in nanoseconds.
 */
unsigned long long currentTimeNanos();

/**
 * @brief Pauses the program for a specified number of milliseconds.
 * @param millis The number of milliseconds to pause.
 */
void sleepMillis(unsigned long millis);

/**
 * @brief Pauses the program for a specified number of nanoseconds.
 * @param nanos The number of nanoseconds to pause.
 */
void sleepNanos(unsigned long long nanos);

// Math

/**
 * @brief Clamps a value between a minimum and maximum.
 * @param value The value to clamp.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value.
 */
double clamp_d(double value, double min, double max);

/**
 * @brief Clamps a value between a minimum and maximum.
 * @param value The value to clamp.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value.
 */
float clamp_f(float value, float min, float max);

/**
 * @brief Clamps a value between a minimum and maximum.
 * @param value The value to clamp.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value.
 */
int clamp_i(int value, int min, int max);

/**
 * @brief Linearly interpolates between two values.
 * @param a The first value.
 * @param b The second value.
 * @param t The interpolation factor.
 * @return The interpolated value.
 */
double lerp_d(double a, double b, double t);

/**
 * @brief Linearly interpolates between two values.
 * @param a The first value.
 * @param b The second value.
 * @param t The interpolation factor.
 * @return The interpolated value.
 */
float lerp_f(float a, float b, float t);

/**
 * @brief Linearly interpolates between two values.
 * @param a The first value.
 * @param b The second value.
 * @param t The interpolation factor.
 * @return The interpolated value.
 */
int lerp_i(int a, int b, double t);

/**
 * @brief Converts an RGB color to HSV.
 * @param rgb The RGB color.
 * @param h The pointer to the hue.
 * @param s The pointer to the saturation.
 * @param v The pointer to the value.
 */
void rgb_to_hsv(int rgb, double* h, double* s, double* v);

/**
 * @brief Converts an HSV color to RGB.
 * @param h The hue.
 * @param s The saturation.
 * @param v The value.
 * @return The RGB color.
 */
int hsv_to_rgb(double h, double s, double v);

/**
 * @brief Linearly interpolates between two colors.
 * @param rgb1 The first color in RGB format.
 * @param rgb2 The second color in RGB format.
 * @param t The interpolation factor.
 * @return The interpolated color.
 */
int lerp_color(int rgb1, int rgb2, double t);

#ifdef __cplusplus
}
#endif