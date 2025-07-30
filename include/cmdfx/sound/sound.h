/**
 * @file sound.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Primary sound engine declarations
 * @version 0.3.0
 * @date 2025-07-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Play a sound file.
 * 
 * Supported formats:
 * - WAV
 * - MP3
 * - OGG
 * - FLAC
 * - AAC
 *
 * Formats should be available on all platforms.
 *
 * @param soundFile The path to the sound file to play.
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_play(const char* soundFile);

/**
 * @brief Play a sound file in a loop.
 * 
 * @param soundFile The path to the sound file to play.
 * @param loopCount The number of times to loop the sound. Use -1 for infinite looping.
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_playLooped(const char* soundFile, int loopCount);

/**
 * @brief Pause a sound file.
 * 
 * @param soundFile The path to the sound file to pause.
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_pause(const char* soundFile);

/**
 * @brief Pause all sound files.
 * 
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_pauseAll();

/**
 * @brief Resume a sound file.
 * 
 * @param soundFile The path to the sound file to resume.
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_resume(const char* soundFile);

/**
 * @brief Resume all sound files.
 * 
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_resumeAll();

/**
 * @brief Stop a sound file.
 * 
 * @param soundFile The path to the sound file to stop.
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_stop(const char* soundFile);

/**
 * @brief Stop all sound files.
 * 
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_stopAll();

/**
 * @brief Get the volume of a sound file.
 * 
 * @param soundFile The path to the sound file.
 * @param volume Pointer to store the volume level (0.0 to 1.0).
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_getVolume(const char* soundFile, double* volume);

/**
 * @brief Get the volume of all sound files.
 * 
 * @param volume Pointer to store the volume level (0.0 to 1.0).
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_getVolumeAll(double* volume);

/**
 * @brief Set the volume of a sound file.
 * 
 * @param soundFile The path to the sound file.
 * @param volume The volume level (0.0 to 1.0).
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_setVolume(const char* soundFile, double volume);

/**
 * @brief Set the volume of all sound files.
 * 
 * @param volume The volume level (0.0 to 1.0).
 * @return int 0 on success, or a negative error code on failure.
 */
int Sound_setVolumeAll(double volume);

#ifdef __cplusplus
}
#endif