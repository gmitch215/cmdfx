#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdfx/sound/sound.h"

// Internal sound management structures
typedef struct _SoundInstance {
    char* filePath;
    int isPlaying;
    int isPaused;
    int loopCount;
    double volume;
    void* platformData;
    struct _SoundInstance* next;
} _SoundInstance;

static _SoundInstance* _soundInstances = NULL;
static double _globalVolume = 1.0;
static int _soundSystemInitialized = 0;

// Forward declarations for platform-specific functions
extern int _Platform_initSoundSystem();
extern void _Platform_shutdownSoundSystem();
extern int _Platform_playSound(const char* soundFile, int loopCount, void** platformData);
extern int _Platform_pauseSound(void* platformData);
extern int _Platform_resumeSound(void* platformData);
extern int _Platform_stopSound(void* platformData);
extern int _Platform_setSoundVolume(void* platformData, double volume);
extern void _Platform_freeSoundData(void* platformData);

// Internal helper functions
static int _initSoundSystem() {
    if (_soundSystemInitialized) return 0;
    
    int result = _Platform_initSoundSystem();
    if (result == 0) {
        _soundSystemInitialized = 1;
    }
    return result;
}

static _SoundInstance* _findSoundInstance(const char* soundFile) {
    if (!soundFile) return NULL;
    
    _SoundInstance* current = _soundInstances;
    while (current) {
        if (current->filePath && strcmp(current->filePath, soundFile) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static _SoundInstance* _createSoundInstance(const char* soundFile) {
    if (!soundFile) return NULL;
    
    _SoundInstance* instance = malloc(sizeof(_SoundInstance));
    if (!instance) return NULL;
    
    instance->filePath = malloc(strlen(soundFile) + 1);
    if (!instance->filePath) {
        free(instance);
        return NULL;
    }
    
    strcpy(instance->filePath, soundFile);
    instance->isPlaying = 0;
    instance->isPaused = 0;
    instance->loopCount = 0;
    instance->volume = 1.0;
    instance->platformData = NULL;
    instance->next = _soundInstances;
    _soundInstances = instance;
    
    return instance;
}

static void _removeSoundInstance(_SoundInstance* instance) {
    if (!instance) return;
    
    if (_soundInstances == instance) {
        _soundInstances = instance->next;
    } else {
        _SoundInstance* current = _soundInstances;
        while (current && current->next != instance) {
            current = current->next;
        }
        if (current) {
            current->next = instance->next;
        }
    }
    
    if (instance->platformData) {
        _Platform_freeSoundData(instance->platformData);
    }
    free(instance->filePath);
    free(instance);
}

// Public API implementations
int Sound_play(const char* soundFile) {
    return Sound_playLooped(soundFile, 1);
}

int Sound_playLooped(const char* soundFile, int loopCount) {
    if (!soundFile) return -1;
    if (loopCount == 0) return -1;
    
    if (_initSoundSystem() != 0) return -2;
    
    _SoundInstance* instance = _findSoundInstance(soundFile);
    if (!instance) {
        instance = _createSoundInstance(soundFile);
        if (!instance) return -3;
    }
    
    // Stop current playback if any
    if (instance->isPlaying) {
        Sound_stop(soundFile);
    }
    
    instance->loopCount = loopCount;
    int result = _Platform_playSound(soundFile, loopCount, &instance->platformData);
    if (result == 0) {
        instance->isPlaying = 1;
        instance->isPaused = 0;
        // Apply current volume settings
        _Platform_setSoundVolume(instance->platformData, instance->volume * _globalVolume);
    }
    
    return result;
}

int Sound_pause(const char* soundFile) {
    if (!soundFile) return -1;
    
    _SoundInstance* instance = _findSoundInstance(soundFile);
    if (!instance || !instance->isPlaying || instance->isPaused) return -1;
    
    int result = _Platform_pauseSound(instance->platformData);
    if (result == 0) {
        instance->isPaused = 1;
    }
    
    return result;
}

int Sound_pauseAll() {
    int totalResult = 0;
    
    _SoundInstance* current = _soundInstances;
    while (current) {
        if (current->isPlaying && !current->isPaused) {
            int result = Sound_pause(current->filePath);
            if (result != 0) totalResult = result;
        }
        current = current->next;
    }
    
    return totalResult;
}

int Sound_resume(const char* soundFile) {
    if (!soundFile) return -1;
    
    _SoundInstance* instance = _findSoundInstance(soundFile);
    if (!instance || !instance->isPlaying || !instance->isPaused) return -1;
    
    int result = _Platform_resumeSound(instance->platformData);
    if (result == 0) {
        instance->isPaused = 0;
    }
    
    return result;
}

int Sound_resumeAll() {
    int totalResult = 0;
    
    _SoundInstance* current = _soundInstances;
    while (current) {
        if (current->isPlaying && current->isPaused) {
            int result = Sound_resume(current->filePath);
            if (result != 0) totalResult = result;
        }
        current = current->next;
    }
    
    return totalResult;
}

int Sound_stop(const char* soundFile) {
    if (!soundFile) return -1;
    
    _SoundInstance* instance = _findSoundInstance(soundFile);
    if (!instance || !instance->isPlaying) return -1;
    
    int result = _Platform_stopSound(instance->platformData);
    if (result == 0) {
        instance->isPlaying = 0;
        instance->isPaused = 0;
    }
    
    return result;
}

int Sound_stopAll() {
    int totalResult = 0;
    
    _SoundInstance* current = _soundInstances;
    while (current) {
        if (current->isPlaying) {
            int result = Sound_stop(current->filePath);
            if (result != 0) totalResult = result;
        }
        current = current->next;
    }
    
    return totalResult;
}

int Sound_getVolume(const char* soundFile, double* volume) {
    if (!soundFile || !volume) return -1;
    
    _SoundInstance* instance = _findSoundInstance(soundFile);
    if (!instance) return -1;
    
    *volume = instance->volume;
    return 0;
}

int Sound_getVolumeAll(double* volume) {
    if (!volume) return -1;
    
    *volume = _globalVolume;
    return 0;
}

int Sound_setVolume(const char* soundFile, double volume) {
    if (!soundFile || volume < 0.0 || volume > 1.0) return -1;
    
    _SoundInstance* instance = _findSoundInstance(soundFile);
    if (!instance) return -1;
    
    instance->volume = volume;
    
    // Apply volume if sound is currently playing
    if (instance->isPlaying && instance->platformData) {
        return _Platform_setSoundVolume(instance->platformData, volume * _globalVolume);
    }
    
    return 0;
}

int Sound_setVolumeAll(double volume) {
    if (volume < 0.0 || volume > 1.0) return -1;
    
    _globalVolume = volume;
    
    // Apply to all currently playing sounds
    _SoundInstance* current = _soundInstances;
    while (current) {
        if (current->isPlaying && current->platformData) {
            _Platform_setSoundVolume(current->platformData, current->volume * _globalVolume);
        }
        current = current->next;
    }
    
    return 0;
}

// Cleanup function (should be called on shutdown)
void Sound_cleanup() {
    Sound_stopAll();
    
    while (_soundInstances) {
        _SoundInstance* next = _soundInstances->next;
        _removeSoundInstance(_soundInstances);
        _soundInstances = next;
    }
    
    if (_soundSystemInitialized) {
        _Platform_shutdownSoundSystem();
        _soundSystemInitialized = 0;
    }
}
