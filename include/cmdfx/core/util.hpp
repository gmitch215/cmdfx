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

extern "C"
{
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

// Multithreading

unsigned long launchThread(void (*func)(void*), void* arg) {
    return CmdFX_launchThread(func, arg);
}

int joinThread(unsigned long thread) {
    return CmdFX_joinThread(thread);
}

int detachThread(unsigned long thread) {
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