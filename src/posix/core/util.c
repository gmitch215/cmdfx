#include <time.h>
#include <sys/time.h>

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