/**
 * @file player.h
 * @author Gregory Mitchell (me@gmitch215.xyz)
 * @brief Audio Player API for CmdFX.
 * @version 0.1.1
 * @date 2025-02-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Plays an audio file.
 * 
 * This method plays an audio file. The file should be in a format that is
 * supported by the system. If an error occurs, the method will return -1.
 * 
 * @param file The path to the audio file.
 * @return 0 if the audio file was played, -1 if an error occurred.
 */
int Player_play(char* file);

#ifdef __cplusplus
}
#endif