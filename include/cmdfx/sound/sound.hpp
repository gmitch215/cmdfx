/**
 * @file sound.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief C++ Extensions for sound engine declarations
 * @version 0.3.0
 * @date 2025-09-02
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

extern "C" {
#include "sound.h"
}

#include <string>

namespace CmdFX
{

/**
 * @brief A C++ Wrapper around the sound engine declarations
 */
namespace Sound
{
bool play(std::string soundFile) {
    return Sound_play(soundFile.c_str()) != 0;
}

bool playLooped(std::string soundFile, int loopCount) {
    return Sound_playLooped(soundFile.c_str(), loopCount) != 0;
}

bool pause(std::string soundFile) {
    return Sound_pause(soundFile.c_str()) != 0;
}

bool pauseAll() {
    return Sound_pauseAll() != 0;
}

bool resume(std::string soundFile) {
    return Sound_resume(soundFile.c_str()) != 0;
}

bool resumeAll() {
    return Sound_resumeAll() != 0;
}

bool stop(std::string soundFile) {
    return Sound_stop(soundFile.c_str()) != 0;
}

bool stopAll() {
    return Sound_stopAll() != 0;
}

double getVolume(std::string soundFile) {
    double volume = 0.0;
    if (Sound_getVolume(soundFile.c_str(), &volume) != 0) {
        return -1.0;
    }

    return volume;
}

double getVolumeAll() {
    double volume = 0.0;
    if (Sound_getVolumeAll(&volume) != 0) {
        return -1.0;
    }

    return volume;
}

void setVolume(std::string soundFile, double volume) {
    Sound_setVolume(soundFile.c_str(), volume);
}

void setVolumeAll(double volume) {
    Sound_setVolumeAll(volume);
}

} // namespace Sound

} // namespace CmdFX
