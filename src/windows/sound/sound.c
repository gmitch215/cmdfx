#define WIN32_LEAN_AND_MEAN // exclude unused APIs
// clang-format off
#include <windows.h>
#include <mmsystem.h>
// clang-format on
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/sound/sound.h"

#pragma comment(lib, "winmm.lib")

// Windows-specific sound data structure
typedef struct _WindowsSoundData {
    HWAVEOUT hWaveOut;
    WAVEFORMATEX waveFormat;
    WAVEHDR waveHeader;
    HANDLE hFile;
    HANDLE hMapFile;
    LPVOID lpMapAddress;
    DWORD fileSize;
    DWORD currentPosition;
    int loopCount;
    int currentLoop;
    double volume;
    int isPlaying;
    int isPaused;
    CRITICAL_SECTION criticalSection;
} _WindowsSoundData;

static int _soundSystemInitialized = 0;

int _Platform_initSoundSystem() {
    if (_soundSystemInitialized) return 0;

    // Windows multimedia system is always available
    _soundSystemInitialized = 1;
    return 0;
}

void _Platform_shutdownSoundSystem() {
    if (!_soundSystemInitialized) return;

    // Cleanup is handled per-instance
    _soundSystemInitialized = 0;
}

// Wave output callback
void CALLBACK _waveOutCallback(
    HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1,
    DWORD_PTR dwParam2
) {
    _WindowsSoundData* soundData = (_WindowsSoundData*) dwInstance;

    if (uMsg == WOM_DONE && soundData) {
        EnterCriticalSection(&soundData->criticalSection);

        if (soundData->isPlaying && !soundData->isPaused) {
            // Check if we should loop
            if (soundData->loopCount == -1 ||
                soundData->currentLoop < soundData->loopCount - 1) {
                soundData->currentLoop++;
                soundData->currentPosition = 0;

                // Reset and play again
                waveOutUnprepareHeader(
                    soundData->hWaveOut, &soundData->waveHeader, sizeof(WAVEHDR)
                );
                soundData->waveHeader.lpData = (LPSTR) soundData->lpMapAddress;
                soundData->waveHeader.dwBufferLength = soundData->fileSize;
                soundData->waveHeader.dwFlags = 0;

                waveOutPrepareHeader(
                    soundData->hWaveOut, &soundData->waveHeader, sizeof(WAVEHDR)
                );
                waveOutWrite(
                    soundData->hWaveOut, &soundData->waveHeader, sizeof(WAVEHDR)
                );
            }
            else {
                soundData->isPlaying = 0;
            }
        }

        LeaveCriticalSection(&soundData->criticalSection);
    }
}

int _Platform_playSound(
    const char* soundFile, int loopCount, void** platformData
) {
    if (!soundFile || !platformData) return -1;

    _WindowsSoundData* soundData = malloc(sizeof(_WindowsSoundData));
    if (!soundData) return -1;

    memset(soundData, 0, sizeof(_WindowsSoundData));
    soundData->loopCount = loopCount;
    soundData->currentLoop = 0;
    soundData->volume = 1.0;
    soundData->isPlaying = 0;
    soundData->isPaused = 0;
    soundData->currentPosition = 0;

    InitializeCriticalSection(&soundData->criticalSection);

    // Convert filename to wide character
    int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, soundFile, -1, NULL, 0);
    LPWSTR wideFileName = malloc(wideCharCount * sizeof(WCHAR));
    if (!wideFileName) {
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }
    MultiByteToWideChar(CP_UTF8, 0, soundFile, -1, wideFileName, wideCharCount);

    // Open file
    soundData->hFile = CreateFileW(
        wideFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL
    );

    free(wideFileName);

    if (soundData->hFile == INVALID_HANDLE_VALUE) {
        fprintf(
            stderr, "Failed to open audio file '%s': %lu\n", soundFile,
            GetLastError()
        );
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }

    // Get file size
    soundData->fileSize = GetFileSize(soundData->hFile, NULL);
    if (soundData->fileSize == INVALID_FILE_SIZE) {
        fprintf(stderr, "Failed to get file size: %lu\n", GetLastError());
        CloseHandle(soundData->hFile);
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }

    // Create file mapping
    soundData->hMapFile =
        CreateFileMapping(soundData->hFile, NULL, PAGE_READONLY, 0, 0, NULL);

    if (!soundData->hMapFile) {
        fprintf(stderr, "Failed to create file mapping: %lu\n", GetLastError());
        CloseHandle(soundData->hFile);
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }

    // Map file to memory
    soundData->lpMapAddress =
        MapViewOfFile(soundData->hMapFile, FILE_MAP_READ, 0, 0, 0);

    if (!soundData->lpMapAddress) {
        fprintf(stderr, "Failed to map file to memory: %lu\n", GetLastError());
        CloseHandle(soundData->hMapFile);
        CloseHandle(soundData->hFile);
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }

    // Set up wave format (assuming 16-bit stereo 44.1kHz WAV)
    soundData->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    soundData->waveFormat.nChannels = 2;
    soundData->waveFormat.nSamplesPerSec = 44100;
    soundData->waveFormat.wBitsPerSample = 16;
    soundData->waveFormat.nBlockAlign =
        soundData->waveFormat.nChannels *
        (soundData->waveFormat.wBitsPerSample / 8);
    soundData->waveFormat.nAvgBytesPerSec =
        soundData->waveFormat.nSamplesPerSec *
        soundData->waveFormat.nBlockAlign;
    soundData->waveFormat.cbSize = 0;

    // Open wave output device
    MMRESULT result = waveOutOpen(
        &soundData->hWaveOut, WAVE_MAPPER, &soundData->waveFormat,
        (DWORD_PTR) _waveOutCallback, (DWORD_PTR) soundData, CALLBACK_FUNCTION
    );

    if (result != MMSYSERR_NOERROR) {
        fprintf(stderr, "Failed to open wave output device: %u\n", result);
        UnmapViewOfFile(soundData->lpMapAddress);
        CloseHandle(soundData->hMapFile);
        CloseHandle(soundData->hFile);
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }

    // Set up wave header
    soundData->waveHeader.lpData = (LPSTR) soundData->lpMapAddress;
    soundData->waveHeader.dwBufferLength = soundData->fileSize;
    soundData->waveHeader.dwFlags = 0;
    soundData->waveHeader.dwLoops = 0;

    // Prepare header
    result = waveOutPrepareHeader(
        soundData->hWaveOut, &soundData->waveHeader, sizeof(WAVEHDR)
    );
    if (result != MMSYSERR_NOERROR) {
        fprintf(stderr, "Failed to prepare wave header: %u\n", result);
        waveOutClose(soundData->hWaveOut);
        UnmapViewOfFile(soundData->lpMapAddress);
        CloseHandle(soundData->hMapFile);
        CloseHandle(soundData->hFile);
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }

    // Set volume
    DWORD volumeValue = (DWORD) (soundData->volume * 0xFFFF);
    waveOutSetVolume(soundData->hWaveOut, MAKELONG(volumeValue, volumeValue));

    // Start playback
    result = waveOutWrite(
        soundData->hWaveOut, &soundData->waveHeader, sizeof(WAVEHDR)
    );
    if (result != MMSYSERR_NOERROR) {
        fprintf(stderr, "Failed to start wave playback: %u\n", result);
        waveOutUnprepareHeader(
            soundData->hWaveOut, &soundData->waveHeader, sizeof(WAVEHDR)
        );
        waveOutClose(soundData->hWaveOut);
        UnmapViewOfFile(soundData->lpMapAddress);
        CloseHandle(soundData->hMapFile);
        CloseHandle(soundData->hFile);
        DeleteCriticalSection(&soundData->criticalSection);
        free(soundData);
        return -1;
    }

    soundData->isPlaying = 1;
    *platformData = soundData;

    return 0;
}

int _Platform_pauseSound(void* platformData) {
    if (!platformData) return -1;

    _WindowsSoundData* soundData = (_WindowsSoundData*) platformData;

    EnterCriticalSection(&soundData->criticalSection);

    if (soundData->isPlaying && !soundData->isPaused) {
        waveOutPause(soundData->hWaveOut);
        soundData->isPaused = 1;
    }

    LeaveCriticalSection(&soundData->criticalSection);

    return 0;
}

int _Platform_resumeSound(void* platformData) {
    if (!platformData) return -1;

    _WindowsSoundData* soundData = (_WindowsSoundData*) platformData;

    EnterCriticalSection(&soundData->criticalSection);

    if (soundData->isPlaying && soundData->isPaused) {
        waveOutRestart(soundData->hWaveOut);
        soundData->isPaused = 0;
    }

    LeaveCriticalSection(&soundData->criticalSection);

    return 0;
}

int _Platform_stopSound(void* platformData) {
    if (!platformData) return -1;

    _WindowsSoundData* soundData = (_WindowsSoundData*) platformData;

    EnterCriticalSection(&soundData->criticalSection);

    if (soundData->isPlaying) {
        waveOutReset(soundData->hWaveOut);
        soundData->isPlaying = 0;
        soundData->isPaused = 0;
        soundData->currentLoop = 0;
        soundData->currentPosition = 0;
    }

    LeaveCriticalSection(&soundData->criticalSection);

    return 0;
}

int _Platform_setSoundVolume(void* platformData, double volume) {
    if (!platformData || volume < 0.0 || volume > 1.0) return -1;

    _WindowsSoundData* soundData = (_WindowsSoundData*) platformData;

    EnterCriticalSection(&soundData->criticalSection);

    soundData->volume = volume;

    // Set hardware volume
    DWORD volumeValue = (DWORD) (volume * 0xFFFF);
    waveOutSetVolume(soundData->hWaveOut, MAKELONG(volumeValue, volumeValue));

    LeaveCriticalSection(&soundData->criticalSection);

    return 0;
}

void _Platform_freeSoundData(void* platformData) {
    if (!platformData) return;

    _WindowsSoundData* soundData = (_WindowsSoundData*) platformData;

    EnterCriticalSection(&soundData->criticalSection);

    if (soundData->isPlaying) {
        waveOutReset(soundData->hWaveOut);
    }

    waveOutUnprepareHeader(
        soundData->hWaveOut, &soundData->waveHeader, sizeof(WAVEHDR)
    );
    waveOutClose(soundData->hWaveOut);

    if (soundData->lpMapAddress) {
        UnmapViewOfFile(soundData->lpMapAddress);
    }

    if (soundData->hMapFile) {
        CloseHandle(soundData->hMapFile);
    }

    if (soundData->hFile && soundData->hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(soundData->hFile);
    }

    LeaveCriticalSection(&soundData->criticalSection);
    DeleteCriticalSection(&soundData->criticalSection);

    free(soundData);
}
