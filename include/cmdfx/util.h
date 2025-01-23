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

// Sleep

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

#ifdef __cplusplus
}
#endif