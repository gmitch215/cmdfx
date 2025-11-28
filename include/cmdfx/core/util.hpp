/**
 * @file util.hpp
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for CmdFX Utilities.
 * @version 0.2.1
 * @date 2025-05-20
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C" {
#include "cmdfx/core/util.h"
}

namespace CmdFX
{

// Global Declarations

int getTickSpeed() {
    return CmdFX_getTickSpeed();
}

int setTickSpeed(int tickspeed) {
    return CmdFX_setTickSpeed(tickspeed);
}

// Time

unsigned long currentTimeMillis() {
    return ::currentTimeMillis();
}

unsigned long long currentTimeNanos() {
    return ::currentTimeNanos();
}

void sleepMillis(unsigned long millis) {
    ::sleepMillis(millis);
}

void sleepNanos(unsigned long long nanos) {
    ::sleepNanos(nanos);
}

// Math

double clamp(double value, double min, double max) {
    return clamp_d(value, min, max);
}

float clamp(float value, float min, float max) {
    return clamp_f(value, min, max);
}

int clamp(int value, int min, int max) {
    return clamp_i(value, min, max);
}

double lerp(double a, double b, double t) {
    return lerp_d(a, b, t);
}

float lerp(float a, float b, float t) {
    return lerp_f(a, b, t);
}

int lerp(int a, int b, double t) {
    return lerp_i(a, b, t);
}

void rgbToHsv(int rgb, double* h, double* s, double* v) {
    rgb_to_hsv(rgb, h, s, v);
}

int hsvToRgb(double h, double s, double v) {
    return hsv_to_rgb(h, s, v);
}

int lerpColor(int rgb1, int rgb2, double t) {
    return lerp_color(rgb1, rgb2, t);
}

// Multithreading

ThreadID launchThread(void (*func)(void*), void* arg) {
    return CmdFX_launchThread(func, arg);
}

int joinThread(ThreadID thread) {
    return CmdFX_joinThread(thread);
}

int detachThread(ThreadID thread) {
    return CmdFX_detachThread(thread);
}

void* getInternalMutex(int id) {
    return CmdFX_getInternalMutex(id);
}

int lockMutex(void* mutex) {
    return CmdFX_lockMutex(mutex);
}

int unlockMutex(void* mutex) {
    return CmdFX_unlockMutex(mutex);
}

void tryLockMutex(int id) {
    CmdFX_tryLockMutex(id);
}

void tryUnlockMutex(int id) {
    CmdFX_tryUnlockMutex(id);
}

int isThreadSafeEnabled() {
    return CmdFX_isThreadSafeEnabled();
}

int initThreadSafe() {
    return CmdFX_initThreadSafe();
}

int destroyThreadSafe() {
    return CmdFX_destroyThreadSafe();
}

} // namespace CmdFX
