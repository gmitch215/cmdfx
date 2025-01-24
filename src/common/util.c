#include <time.h>

#include "cmdfx/util.h"

// Time

unsigned long currentTimeMillis() {
    return (int) (clock() / (CLOCKS_PER_SEC / 1000));
}

// Math

double clamp_d(double value, double min, double max) {
    return value < min ? min : value > max ? max : value;
}

float clamp_f(float value, float min, float max) {
    return value < min ? min : value > max ? max : value;
}

int clamp_i(int value, int min, int max) {
    return value < min ? min : value > max ? max : value;
}

double lerp_d(double a, double b, double t) {
    return a + (b - a) * t;
}

float lerp_f(float a, float b, float t) {
    return a + (b - a) * t;
}

int lerp_i(int a, int b, double t) {
    return a + (int) ((b - a) * t);
}