#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#include "cmdfx/core/util.h"

// Time

unsigned long currentTimeMillis() {
    struct timeval tv;
    if (gettimeofday(&tv, 0) == 0)
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    return 0;
}

unsigned long long currentTimeNanos() {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
        return ts.tv_nsec;
    
    return 0;
}

// Sleep

void sleepMillis(unsigned long millis) {
    struct timespec ts;
    ts.tv_sec = millis / 1000;
    ts.tv_nsec = (millis % 1000) * 1000000L;
    nanosleep(&ts, 0);
}

void sleepNanos(unsigned long long nanos) {
    struct timespec ts;
    ts.tv_sec = nanos / 1000000000L;
    ts.tv_nsec = nanos % 1000000000L;
    nanosleep(&ts, 0);
}

// Multithreading

static int _threadSafeEnabled = 0;
static void** _mutexes = 0;

void* _createMutex() {
    pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
    if (mutex == 0) return 0;
    
    if (pthread_mutex_init(mutex, 0) != 0) {
        free(mutex);
        return 0;
    }
    
    return mutex;
}

int _destroyMutex(void* mutex) {
    if (mutex == 0) return -1;
    
    pthread_mutex_t* m = (pthread_mutex_t*) mutex;
    if (pthread_mutex_destroy(m) != 0) return -1;
    
    free(m);
    return 0;
}

int CmdFX_initThreadSafe() {
    if (_threadSafeEnabled != 0) return -1;
    if (_mutexes != 0) return -1;
    
    _threadSafeEnabled = 1;
    _mutexes = calloc(MAX_INTERNAL_CMDFX_MUTEXES, sizeof(void*));
    if (_mutexes == 0) return -1;

    for (int i = 0; i < MAX_INTERNAL_CMDFX_MUTEXES; i++) {
        _mutexes[i] = _createMutex();
        if (_mutexes[i] == 0) {
            for (int j = 0; j < i; j++) _destroyMutex(_mutexes[j]);
            free(_mutexes);
            return -1;
        }
    }
    
    return 0;
}

int CmdFX_isThreadSafeEnabled() {
    return _threadSafeEnabled;
}

int CmdFX_destroyThreadSafe() {
    if (_threadSafeEnabled == 0) return -1;
    if (_mutexes == 0) return -1;
    _threadSafeEnabled = 0;
    
    for (int i = 0; i < MAX_INTERNAL_CMDFX_MUTEXES; i++) {
        if (_mutexes[i] != 0) _destroyMutex(_mutexes[i]);
    }
    
    free(_mutexes);
    _mutexes = 0;
    
    return 0;
}

void* CmdFX_getInternalMutex(int index) {
    if (!_threadSafeEnabled) return 0;
    if (index < 0 || index >= MAX_INTERNAL_CMDFX_MUTEXES) return 0;   
    if (_mutexes == 0) return 0;
    
    return _mutexes[index];
}

int CmdFX_lockMutex(void* mutex) {
    if (!_threadSafeEnabled) return -1;
    if (mutex == 0) return -1;
    
    pthread_mutex_t* m = (pthread_mutex_t*) mutex;
    if (pthread_mutex_lock(m) != 0) return -1;
    
    return 0;
}

int CmdFX_unlockMutex(void* mutex) {
    if (!_threadSafeEnabled) return -1;
    if (mutex == 0) return -1;
    
    pthread_mutex_t* m = (pthread_mutex_t*) mutex;
    if (pthread_mutex_unlock(m) != 0) return -1;
    
    return 0;
}

ThreadID CmdFX_launchThread(void (*func)(void*), void* arg) {
    if (!_threadSafeEnabled) return 0;
    
    pthread_t thread;
    if (pthread_create(&thread, 0, (void* (*)(void*))func, arg) != 0) return 0;
    
    return thread;
}

int CmdFX_joinThread(ThreadID thread) {
    if (!_threadSafeEnabled) return -1;
    
    pthread_join(thread, 0);
    return 0;
}

int CmdFX_detachThread(ThreadID thread) {
    if (!_threadSafeEnabled) return -1;
    
    pthread_detach(thread);
    return 0;
}