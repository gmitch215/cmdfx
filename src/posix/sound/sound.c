#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Enable ALSA only if headers are available (Linux)
#if defined(__linux__)
    #if defined(__has_include)
        #if __has_include(<alsa/asoundlib.h>)
            #define CMDFX_HAVE_ALSA 1
            #include <alsa/asoundlib.h>
        #endif
    #endif
#endif

#include "cmdfx/sound/sound.h"

// POSIX-specific sound data structure
typedef struct _PosixSoundData {
#ifdef CMDFX_HAVE_ALSA
    snd_pcm_t* pcm_handle;
    snd_pcm_hw_params_t* hw_params;
#endif
    FILE* audioFile;
    char* audioBuffer;
    size_t bufferSize;
    size_t currentPosition;
    int loopCount;
    int currentLoop;
    double volume;
    int isPlaying;
    int isPaused;
    pthread_t playbackThread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int shouldStop;
} _PosixSoundData;

static int _soundSystemInitialized = 0;
static pthread_mutex_t _systemMutex = PTHREAD_MUTEX_INITIALIZER;

int _Platform_initSoundSystem() {
    pthread_mutex_lock(&_systemMutex);

    if (_soundSystemInitialized) {
        pthread_mutex_unlock(&_systemMutex);
        return 0;
    }

#ifdef CMDFX_HAVE_ALSA
    snd_pcm_t* test_handle;
    int err = snd_pcm_open(
        &test_handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK
    );
    if (err < 0) {
        fprintf(stderr, "Failed to initialize ALSA: %s\n", snd_strerror(err));
        pthread_mutex_unlock(&_systemMutex);
        return -1;
    }
    snd_pcm_close(test_handle);
#endif

    _soundSystemInitialized = 1;
    pthread_mutex_unlock(&_systemMutex);
    return 0;
}

void _Platform_shutdownSoundSystem() {
    pthread_mutex_lock(&_systemMutex);
    _soundSystemInitialized = 0;
    pthread_mutex_unlock(&_systemMutex);
}

// Simple WAV header parser
typedef struct {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char subchunk2_id[4];
    uint32_t subchunk2_size;
} WaveHeader;

// Playback thread function
void* _playbackThread(void* arg) {
    _PosixSoundData* soundData = (_PosixSoundData*) arg;

    pthread_mutex_lock(&soundData->mutex);

    while (soundData->isPlaying && !soundData->shouldStop) {
        if (soundData->isPaused) {
            pthread_cond_wait(&soundData->cond, &soundData->mutex);
            continue;
        }

#ifdef CMDFX_HAVE_ALSA
        if (soundData->pcm_handle) {
            snd_pcm_sframes_t frames_to_write = 1024; // Chunk size
            size_t bytes_to_write =
                frames_to_write * 4; // Assuming 16-bit stereo

            if (soundData->currentPosition + bytes_to_write >
                soundData->bufferSize) {
                bytes_to_write =
                    soundData->bufferSize - soundData->currentPosition;
                frames_to_write = bytes_to_write / 4;
            }

            if (bytes_to_write > 0) {
                // Apply volume
                short* samples = (short*) (soundData->audioBuffer +
                                           soundData->currentPosition);
                size_t sample_count = bytes_to_write / sizeof(short);

                if (soundData->volume != 1.0) {
                    for (size_t i = 0; i < sample_count; i++) {
                        samples[i] = (short) (samples[i] * soundData->volume);
                    }
                }

                snd_pcm_sframes_t frames_written = snd_pcm_writei(
                    soundData->pcm_handle, samples, frames_to_write
                );

                if (frames_written > 0) {
                    soundData->currentPosition += frames_written * 4;
                }
                else if (frames_written == -EPIPE) {
                    // Buffer underrun
                    snd_pcm_prepare(soundData->pcm_handle);
                }
            }

            // Check if we've reached the end
            if (soundData->currentPosition >= soundData->bufferSize) {
                if (soundData->loopCount == -1 ||
                    soundData->currentLoop < soundData->loopCount - 1) {
                    soundData->currentPosition =
                        sizeof(WaveHeader); // Skip header
                    soundData->currentLoop++;
                }
                else {
                    soundData->isPlaying = 0;
                    break;
                }
            }
        }
#else
        usleep(23000); // ~23ms for roughly 44.1kHz timing

        size_t chunk_size = 1024 * 4; // Simulate processing chunks
        soundData->currentPosition += chunk_size;

        if (soundData->currentPosition >= soundData->bufferSize) {
            if (soundData->loopCount == -1 ||
                soundData->currentLoop < soundData->loopCount - 1) {
                soundData->currentPosition = sizeof(WaveHeader);
                soundData->currentLoop++;
            }
            else {
                soundData->isPlaying = 0;
                break;
            }
        }
#endif

        pthread_mutex_unlock(&soundData->mutex);
        usleep(1000); // Small delay to prevent busy waiting
        pthread_mutex_lock(&soundData->mutex);
    }

    pthread_mutex_unlock(&soundData->mutex);
    return NULL;
}

int _Platform_playSound(
    const char* soundFile, int loopCount, void** platformData
) {
    if (!soundFile || !platformData) return -1;

    _PosixSoundData* soundData = malloc(sizeof(_PosixSoundData));
    if (!soundData) return -1;

    memset(soundData, 0, sizeof(_PosixSoundData));
    soundData->loopCount = loopCount;
    soundData->currentLoop = 0;
    soundData->volume = 1.0;
    soundData->isPlaying = 0;
    soundData->isPaused = 0;
    soundData->shouldStop = 0;

    pthread_mutex_init(&soundData->mutex, NULL);
    pthread_cond_init(&soundData->cond, NULL);

    // Open and read audio file
    soundData->audioFile = fopen(soundFile, "rb");
    if (!soundData->audioFile) {
        fprintf(stderr, "Failed to open audio file '%s'\n", soundFile);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    // Get file size
    fseek(soundData->audioFile, 0, SEEK_END);
    soundData->bufferSize = ftell(soundData->audioFile);
    fseek(soundData->audioFile, 0, SEEK_SET);

    // Read entire file into memory
    soundData->audioBuffer = malloc(soundData->bufferSize);
    if (!soundData->audioBuffer) {
        fprintf(stderr, "Failed to allocate audio buffer\n");
        fclose(soundData->audioFile);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    if (fread(
            soundData->audioBuffer, 1, soundData->bufferSize,
            soundData->audioFile
        ) != soundData->bufferSize) {
        fprintf(stderr, "Failed to read audio file\n");
        free(soundData->audioBuffer);
        fclose(soundData->audioFile);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    fclose(soundData->audioFile);
    soundData->audioFile = NULL;

    // Parse WAV header to skip it
    if (soundData->bufferSize >= sizeof(WaveHeader)) {
        WaveHeader* header = (WaveHeader*) soundData->audioBuffer;
        if (strncmp(header->chunk_id, "RIFF", 4) == 0 &&
            strncmp(header->format, "WAVE", 4) == 0) {
            soundData->currentPosition = sizeof(WaveHeader);
        }
    }

#ifdef CMDFX_HAVE_ALSA
    // Initialize ALSA
    int err = snd_pcm_open(
        &soundData->pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0
    );
    if (err < 0) {
        fprintf(stderr, "Failed to open ALSA device: %s\n", snd_strerror(err));
        free(soundData->audioBuffer);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    // Allocate hardware parameters
    err = snd_pcm_hw_params_malloc(&soundData->hw_params);
    if (err < 0) {
        fprintf(
            stderr, "Failed to allocate hw params: %s\n", snd_strerror(err)
        );
        snd_pcm_close(soundData->pcm_handle);
        free(soundData->audioBuffer);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    // Initialize hardware parameters
    err = snd_pcm_hw_params_any(soundData->pcm_handle, soundData->hw_params);
    if (err < 0) {
        fprintf(
            stderr, "Failed to initialize hw params: %s\n", snd_strerror(err)
        );
        snd_pcm_hw_params_free(soundData->hw_params);
        snd_pcm_close(soundData->pcm_handle);
        free(soundData->audioBuffer);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    // Set hardware parameters (assuming 16-bit stereo 44.1kHz)
    snd_pcm_hw_params_set_access(
        soundData->pcm_handle, soundData->hw_params,
        SND_PCM_ACCESS_RW_INTERLEAVED
    );
    snd_pcm_hw_params_set_format(
        soundData->pcm_handle, soundData->hw_params, SND_PCM_FORMAT_S16_LE
    );
    snd_pcm_hw_params_set_channels(
        soundData->pcm_handle, soundData->hw_params, 2
    );
    unsigned int rate = 44100;
    snd_pcm_hw_params_set_rate_near(
        soundData->pcm_handle, soundData->hw_params, &rate, 0
    );

    // Apply hardware parameters
    err = snd_pcm_hw_params(soundData->pcm_handle, soundData->hw_params);
    if (err < 0) {
        fprintf(stderr, "Failed to set hw params: %s\n", snd_strerror(err));
        snd_pcm_hw_params_free(soundData->hw_params);
        snd_pcm_close(soundData->pcm_handle);
        free(soundData->audioBuffer);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    snd_pcm_hw_params_free(soundData->hw_params);
    soundData->hw_params = NULL;

    // Prepare PCM device
    err = snd_pcm_prepare(soundData->pcm_handle);
    if (err < 0) {
        fprintf(
            stderr, "Failed to prepare PCM device: %s\n", snd_strerror(err)
        );
        snd_pcm_close(soundData->pcm_handle);
        free(soundData->audioBuffer);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }
#endif

    // Start playback thread
    soundData->isPlaying = 1;
    if (pthread_create(
            &soundData->playbackThread, NULL, _playbackThread, soundData
        ) != 0) {
        fprintf(stderr, "Failed to create playback thread\n");

#ifdef CMDFX_HAVE_ALSA
        snd_pcm_close(soundData->pcm_handle);
#endif

        free(soundData->audioBuffer);
        pthread_mutex_destroy(&soundData->mutex);
        pthread_cond_destroy(&soundData->cond);
        free(soundData);
        return -1;
    }

    *platformData = soundData;
    return 0;
}

int _Platform_pauseSound(void* platformData) {
    if (!platformData) return -1;

    _PosixSoundData* soundData = (_PosixSoundData*) platformData;

    pthread_mutex_lock(&soundData->mutex);
    soundData->isPaused = 1;
    pthread_mutex_unlock(&soundData->mutex);

    return 0;
}

int _Platform_resumeSound(void* platformData) {
    if (!platformData) return -1;

    _PosixSoundData* soundData = (_PosixSoundData*) platformData;

    pthread_mutex_lock(&soundData->mutex);
    soundData->isPaused = 0;
    pthread_cond_signal(&soundData->cond);
    pthread_mutex_unlock(&soundData->mutex);

    return 0;
}

int _Platform_stopSound(void* platformData) {
    if (!platformData) return -1;

    _PosixSoundData* soundData = (_PosixSoundData*) platformData;

    pthread_mutex_lock(&soundData->mutex);
    soundData->isPlaying = 0;
    soundData->shouldStop = 1;
    pthread_cond_signal(&soundData->cond);
    pthread_mutex_unlock(&soundData->mutex);

    // Wait for thread to finish
    pthread_join(soundData->playbackThread, NULL);

    return 0;
}

int _Platform_setSoundVolume(void* platformData, double volume) {
    if (!platformData || volume < 0.0 || volume > 1.0) return -1;

    _PosixSoundData* soundData = (_PosixSoundData*) platformData;

    pthread_mutex_lock(&soundData->mutex);
    soundData->volume = volume;
    pthread_mutex_unlock(&soundData->mutex);

    return 0;
}

void _Platform_freeSoundData(void* platformData) {
    if (!platformData) return;

    _PosixSoundData* soundData = (_PosixSoundData*) platformData;

    // Stop playback if still running
    if (soundData->isPlaying) {
        _Platform_stopSound(platformData);
    }

#ifdef CMDFX_HAVE_ALSA
    if (soundData->pcm_handle) {
        snd_pcm_close(soundData->pcm_handle);
    }
#endif

    if (soundData->audioBuffer) {
        free(soundData->audioBuffer);
    }

    if (soundData->audioFile) {
        fclose(soundData->audioFile);
    }

    pthread_mutex_destroy(&soundData->mutex);
    pthread_cond_destroy(&soundData->cond);

    free(soundData);
}
