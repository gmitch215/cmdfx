#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/sound/sound.h"

// macOS-specific sound data structure
typedef struct _MacOSSoundData {
    AudioUnit audioUnit;
    AudioFileID audioFile;
    AudioStreamBasicDescription fileFormat;
    UInt64 totalFrames;
    UInt64 currentFrame;
    int loopCount;
    int currentLoop;
    double volume;
    int isPlaying;
    int isPaused;
} _MacOSSoundData;

static int _soundSystemInitialized = 0;

int _Platform_initSoundSystem() {
    if (_soundSystemInitialized) return 0;

    // Initialize Core Audio
    AudioComponentInstance* out = NULL;
    OSStatus status = AudioComponentInstanceNew(
        AudioComponentFindNext(
            NULL, &(AudioComponentDescription
                  ) {.componentType = kAudioUnitType_Output,
                     .componentSubType = kAudioUnitSubType_DefaultOutput,
                     .componentManufacturer = kAudioUnitManufacturer_Apple}
        ),
        out
    );

    if (status != noErr) {
        fprintf(
            stderr, "Failed to initialize Core Audio system: %d\n", (int) status
        );
        return -1;
    }

    _soundSystemInitialized = 1;
    return 0;
}

void _Platform_shutdownSoundSystem() {
    if (!_soundSystemInitialized) return;

    // Core Audio cleanup is handled per-instance
    _soundSystemInitialized = 0;
}

// Audio render callback
static OSStatus _audioRenderCallback(
    void* inRefCon, AudioUnitRenderActionFlags* ioActionFlags,
    const AudioTimeStamp* inTimeStamp, UInt32 inBusNumber,
    UInt32 inNumberFrames, AudioBufferList* ioData
) {
    _MacOSSoundData* soundData = (_MacOSSoundData*) inRefCon;

    if (!soundData || !soundData->isPlaying || soundData->isPaused) {
        // Silence
        for (UInt32 i = 0; i < ioData->mNumberBuffers; i++) {
            memset(
                ioData->mBuffers[i].mData, 0, ioData->mBuffers[i].mDataByteSize
            );
        }
        return noErr;
    }

    UInt32 bytesToRead = ioData->mBuffers[0].mDataByteSize;
    UInt32 bytesRead = 0;

    OSStatus status = AudioFileReadBytes(
        soundData->audioFile, false,
        soundData->currentFrame * soundData->fileFormat.mBytesPerFrame,
        &bytesToRead, ioData->mBuffers[0].mData
    );

    if (status == noErr) {
        bytesRead = bytesToRead;
        soundData->currentFrame +=
            bytesRead / soundData->fileFormat.mBytesPerFrame;

        // Apply volume
        if (soundData->volume != 1.0) {
            float* samples = (float*) ioData->mBuffers[0].mData;
            UInt32 sampleCount = bytesRead / sizeof(float);
            for (UInt32 i = 0; i < sampleCount; i++) {
                samples[i] *= soundData->volume;
            }
        }

        // Check if we've reached the end
        if (soundData->currentFrame >= soundData->totalFrames) {
            if (soundData->loopCount == -1 ||
                soundData->currentLoop < soundData->loopCount - 1) {
                soundData->currentFrame = 0; // Reset to beginning
                soundData->currentLoop++;
            }
            else {
                soundData->isPlaying = 0; // Stop playback
            }
        }
    }
    else {
        // Error reading file, stop playback
        soundData->isPlaying = 0;
        memset(ioData->mBuffers[0].mData, 0, ioData->mBuffers[0].mDataByteSize);
    }

    return noErr;
}

int _Platform_playSound(
    const char* soundFile, int loopCount, void** platformData
) {
    if (!soundFile || !platformData) return -1;

    _MacOSSoundData* soundData = malloc(sizeof(_MacOSSoundData));
    if (!soundData) return -1;

    memset(soundData, 0, sizeof(_MacOSSoundData));
    soundData->loopCount = loopCount;
    soundData->currentLoop = 0;
    soundData->volume = 1.0;
    soundData->isPlaying = 0;
    soundData->isPaused = 0;

    // Open audio file
    CFStringRef filePath = CFStringCreateWithCString(
        kCFAllocatorDefault, soundFile, kCFStringEncodingUTF8
    );
    CFURLRef fileURL = CFURLCreateWithFileSystemPath(
        kCFAllocatorDefault, filePath, kCFURLPOSIXPathStyle, false
    );

    OSStatus status = AudioFileOpenURL(
        fileURL, kAudioFileReadPermission, 0, &soundData->audioFile
    );
    CFRelease(fileURL);
    CFRelease(filePath);

    if (status != noErr) {
        fprintf(
            stderr, "Failed to open audio file '%s': %d\n", soundFile,
            (int) status
        );
        free(soundData);
        return -1;
    }

    // Get audio format
    UInt32 propSize = sizeof(soundData->fileFormat);
    status = AudioFileGetProperty(
        soundData->audioFile, kAudioFilePropertyDataFormat, &propSize,
        &soundData->fileFormat
    );

    if (status != noErr) {
        fprintf(stderr, "Failed to get audio file format: %d\n", (int) status);
        AudioFileClose(soundData->audioFile);
        free(soundData);
        return -1;
    }

    // Get total frames
    propSize = sizeof(soundData->totalFrames);
    status = AudioFileGetProperty(
        soundData->audioFile, kAudioFilePropertyAudioDataPacketCount, &propSize,
        &soundData->totalFrames
    );

    if (status != noErr) {
        fprintf(
            stderr, "Failed to get audio file frame count: %d\n", (int) status
        );
        AudioFileClose(soundData->audioFile);
        free(soundData);
        return -1;
    }

    // Create audio unit
    AudioComponentDescription desc = {
        .componentType = kAudioUnitType_Output,
        .componentSubType = kAudioUnitSubType_DefaultOutput,
        .componentManufacturer = kAudioUnitManufacturer_Apple
    };

    AudioComponent component = AudioComponentFindNext(NULL, &desc);
    if (!component) {
        fprintf(stderr, "Failed to find audio component\n");
        AudioFileClose(soundData->audioFile);
        free(soundData);
        return -1;
    }

    status = AudioComponentInstanceNew(component, &soundData->audioUnit);
    if (status != noErr) {
        fprintf(stderr, "Failed to create audio unit: %d\n", (int) status);
        AudioFileClose(soundData->audioFile);
        free(soundData);
        return -1;
    }

    // Set render callback
    AURenderCallbackStruct callback = {
        .inputProc = _audioRenderCallback, .inputProcRefCon = soundData
    };

    status = AudioUnitSetProperty(
        soundData->audioUnit, kAudioUnitProperty_SetRenderCallback,
        kAudioUnitScope_Input, 0, &callback, sizeof(callback)
    );

    if (status != noErr) {
        fprintf(stderr, "Failed to set render callback: %d\n", (int) status);
        AudioComponentInstanceDispose(soundData->audioUnit);
        AudioFileClose(soundData->audioFile);
        free(soundData);
        return -1;
    }

    // Initialize and start audio unit
    status = AudioUnitInitialize(soundData->audioUnit);
    if (status != noErr) {
        fprintf(stderr, "Failed to initialize audio unit: %d\n", (int) status);
        AudioComponentInstanceDispose(soundData->audioUnit);
        AudioFileClose(soundData->audioFile);
        free(soundData);
        return -1;
    }

    status = AudioOutputUnitStart(soundData->audioUnit);
    if (status != noErr) {
        fprintf(stderr, "Failed to start audio unit: %d\n", (int) status);
        AudioUnitUninitialize(soundData->audioUnit);
        AudioComponentInstanceDispose(soundData->audioUnit);
        AudioFileClose(soundData->audioFile);
        free(soundData);
        return -1;
    }

    soundData->isPlaying = 1;
    *platformData = soundData;

    return 0;
}

int _Platform_pauseSound(void* platformData) {
    if (!platformData) return -1;

    _MacOSSoundData* soundData = (_MacOSSoundData*) platformData;
    soundData->isPaused = 1;

    return 0;
}

int _Platform_resumeSound(void* platformData) {
    if (!platformData) return -1;

    _MacOSSoundData* soundData = (_MacOSSoundData*) platformData;
    soundData->isPaused = 0;

    return 0;
}

int _Platform_stopSound(void* platformData) {
    if (!platformData) return -1;

    _MacOSSoundData* soundData = (_MacOSSoundData*) platformData;
    soundData->isPlaying = 0;
    soundData->isPaused = 0;

    AudioOutputUnitStop(soundData->audioUnit);

    return 0;
}

int _Platform_setSoundVolume(void* platformData, double volume) {
    if (!platformData || volume < 0.0 || volume > 1.0) return -1;

    _MacOSSoundData* soundData = (_MacOSSoundData*) platformData;
    soundData->volume = volume;

    return 0;
}

void _Platform_freeSoundData(void* platformData) {
    if (!platformData) return;

    _MacOSSoundData* soundData = (_MacOSSoundData*) platformData;

    if (soundData->isPlaying) {
        AudioOutputUnitStop(soundData->audioUnit);
    }

    AudioUnitUninitialize(soundData->audioUnit);
    AudioComponentInstanceDispose(soundData->audioUnit);
    AudioFileClose(soundData->audioFile);

    free(soundData);
}
