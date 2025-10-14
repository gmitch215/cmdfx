#include <process.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "cmdfx/core/util.h"

// Time

unsigned long currentTimeMillis() {
    return (int) (clock() / (CLOCKS_PER_SEC / 1000));
}

unsigned long long currentTimeNanos() {
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);

    ULARGE_INTEGER time;
    time.LowPart = ft.dwLowDateTime;
    time.HighPart = ft.dwHighDateTime;

    return time.QuadPart * 100;
}

// Sleep

void sleepMillis(unsigned long millis) {
    Sleep(millis);
}

void sleepNanos(unsigned long long nanos) {
    HANDLE timer;
    LARGE_INTEGER li;

    // Convert nanoseconds to 100-nanosecond intervals for Windows
    li.QuadPart = -1 * (nanos / 100);

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (!timer) {
        fprintf(stderr, "Failed to create timer\n");
        return;
    }

    if (!SetWaitableTimer(timer, &li, 0, 0, 0, FALSE)) {
        fprintf(stderr, "Failed to set timer\n");
        CloseHandle(timer);
        return;
    }

    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

// Multithreading

static void** _mutexes = 0;
static int _threadSafeEnabled = 0;

void* _createMutex() {
    HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex == NULL) {
        fprintf(stderr, "Failed to create mutex\n");
        return NULL;
    }
    return (void*) mutex;
}

int _destroyMutex(void* mutex) {
    if (mutex == NULL) return -1;

    HANDLE m = (HANDLE) mutex;
    if (!CloseHandle(m)) {
        fprintf(stderr, "Failed to destroy mutex\n");
        return -1;
    }
    return 0;
}

int CmdFX_initThreadSafe() {
    if (_mutexes != 0) return -1;

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
    if (_mutexes == 0) return -1;

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
    if (mutex == NULL) return -1;

    HANDLE m = (HANDLE) mutex;
    DWORD result = WaitForSingleObject(m, INFINITE);
    if (result == WAIT_FAILED) {
        fprintf(stderr, "Failed to lock mutex\n");
        return -1;
    }
    return 0;
}

int CmdFX_unlockMutex(void* mutex) {
    if (!_threadSafeEnabled) return -1;
    if (mutex == NULL) return -1;

    HANDLE m = (HANDLE) mutex;
    if (!ReleaseMutex(m)) {
        fprintf(stderr, "Failed to unlock mutex\n");
        return -1;
    }
    return 0;
}

ThreadID CmdFX_launchThread(void (*func)(void*), void* arg) {
    if (!_threadSafeEnabled) return 0;

    uintptr_t thread = _beginthreadex(
        NULL, 0, (unsigned(__stdcall*)(void*)) func, arg, 0, NULL
    );
    if (thread == 0) {
        fprintf(stderr, "Failed to launch thread\n");
        return 0;
    }

    return thread;
}

int CmdFX_joinThread(ThreadID thread) {
    if (!_threadSafeEnabled) return -1;

    HANDLE hThread = (HANDLE) thread;
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}

int CmdFX_detachThread(ThreadID thread) {
    if (!_threadSafeEnabled) return -1;

    HANDLE hThread = (HANDLE) thread;
    CloseHandle(hThread);

    return 0;
}