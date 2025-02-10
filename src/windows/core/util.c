#include <stdio.h>
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