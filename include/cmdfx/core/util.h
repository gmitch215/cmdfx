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

// Multithreading

/**
 * @brief Maximum number of internal mutexes.
 * 
 * This constant defines the maximum number of internal mutexes
 * that can be created. It is used to limit the number of mutexes
 * that can be created in the system.
 */
#define MAX_INTERNAL_CMDFX_MUTEXES 128

/**
 * @brief Initializes thread-safe functions.
 * 
 * This method initializes internal mutexes and ensures thread-safe
 * operations. It should be called before using any unsafe functions
 * that may be accessed by multiple threads or at the start of the
 * program.
 * 
 * @return 0 if successful, -1 if an error occurred.
 */
int CmdFX_initThreadSafe();

/**
 * @brief Checks if thread-safe functions are enabled.
 * 
 * This method checks if thread-safe functions are enabled. It
 * returns 1 if they are enabled, 0 if they are not.
 * 
 * @return 1 if thread-safe functions are enabled, 0 otherwise.
 */
int CmdFX_isThreadSafeEnabled();

/**
 * @brief Cleans up thread-safe functions.
 * 
 * This method cleans up internal mutexes and ensures that all
 * thread-safe operations are properly terminated. It should be
 * called after all unsafe functions have been used or at the
 * end of the program.
 * 
 * @return 0 if successful, -1 if an error occurred.
 */
int CmdFX_destroyThreadSafe();

/**
 * @brief Gets an internal mutex.
 * 
 * This method gets an internal mutex that can be used for
 * synchronizing access to shared resources. The index
 * should be between 0 and _MAX_INTERNAL_CMDFX_MUTEXES - 1.
 * 
 * @param index The index of the mutex to get.
 * @return Pointer to the platform-specific mutex object.
 */
void* CmdFX_getInternalMutex(int index);

/**
 * @brief Locks a mutex.
 * 
 * This method locks a mutex, blocking the calling thread until
 * the mutex is available.
 * 
 * @param mutex Pointer to the platform-specific mutex object.
 * @return 0 if successful, -1 if an error occurred.
 */
int CmdFX_lockMutex(void* mutex);

/**
 * @brief Unlocks a mutex.
 * 
 * This method unlocks a mutex, allowing other threads to access
 * the shared resource.
 * 
 * @param mutex Pointer to the platform-specific mutex object.
 * @return 0 if successful, -1 if an error occurred.
 */
int CmdFX_unlockMutex(void* mutex);

/**
 * @brief Tries to lock a mutex.
 * 
 * This method tries to lock a mutex without blocking the calling
 * thread. If the mutex is already locked, it returns immediately.
 * 
 * It will also initialize the mutex if it is not already initialized.
 * 
 * @param id The ID of the mutex to lock.
 */
void CmdFX_tryLockMutex(int id);

/**
 * @brief Tries to unlock a mutex.
 * 
 * This method tries to unlock a mutex without blocking the calling
 * thread. If the mutex is already unlocked, it returns immediately.
 * 
 * @param id The ID of the mutex to unlock.
 */
void CmdFX_tryUnlockMutex(int id);

#ifdef _WIN32
#include <stdint.h>
#define ThreadID uintptr_t
#else
#include <pthread.h>
#define ThreadID pthread_t
#endif

/**
 * @brief Launches a thread.
 * 
 * This method launches a thread that runs the specified function
 * with the specified argument.
 * 
 * To run the thread in the background, use the `CmdFX_detachThread`
 * function. You must call `CmdFX_initThreadSafe` before using this
 * function.
 * 
 * @param func The function to run in the thread.
 * @param arg The argument to pass to the function.
 * @return The thread identifier.
 */
ThreadID CmdFX_launchThread(void (*func)(void*), void* arg);

/**
 * @brief Joins a thread.
 * 
 * This method joins a thread, blocking the calling thread until
 * the specified thread has completed its execution.
 * 
 * You must call `CmdFX_initThreadSafe` before using this
 * function.
 * 
 * @param thread The thread identifier.
 * @return 0 if successful, -1 if an error occurred.
 */
int CmdFX_joinThread(ThreadID thread);

/**
 * @brief Detaches a thread.
 * 
 * This method detaches a thread, allowing it to run independently
 * of the main thread. The calling thread will not block until the
 * detached thread has completed its execution.
 * 
 * You must call `CmdFX_initThreadSafe` before using this
 * function.
 * 
 * @param thread The pointer to the thread identifier.
 * @return 0 if successful, -1 if an error occurred.
 */
int CmdFX_detachThread(ThreadID thread);

#ifdef __cplusplus
}
#endif