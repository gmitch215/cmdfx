#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

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

void rgb_to_hsv(int rgb, double* h, double* s, double* v) {
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;

    double min = fmin(fmin(r, g), b);
    double max = fmax(fmax(r, g), b);
    *v = max;

    double delta = max - min;

    if (delta < 0.00001) {
        *s = 0;
        *h = 0;
        return;
    }

    if (max > 0) {
        *s = (delta / max);
    } else {
        *s = 0;
        *h = NAN;
        return;
    }

    if (r >= max) {
        *h = (g - b) / delta;
    } else if (g >= max) {
        *h = 2 + (b - r) / delta;
    } else {
        *h = 4 + (r - g) / delta;
    }

    *h *= 60;

    if (*h < 0)
        *h += 360;
}

int hsv_to_rgb(double h, double s, double v) {
    if (s <= 0) {
        int value = (int) (v * 255);
        return (value << 16) | (value << 8) | value;
    }

    double hh = h;
    long i = (long) hh / 60;
    double p = v * (1 - s);
    double q = v * (1 - s * (hh / 60 - i));
    double t = v * (1 - s * (1 - (hh / 60 - i)));

    switch (i) {
        case 0:
            return ((int) (v * 255) << 16) | ((int) (t * 255) << 8) | (int) (p * 255);
        case 1:
            return ((int) (q * 255) << 16) | ((int) (v * 255) << 8) | (int) (p * 255);
        case 2:
            return ((int) (p * 255) << 16) | ((int) (v * 255) << 8) | (int) (t * 255);
        case 3:
            return ((int) (p * 255) << 16) | ((int) (q * 255) << 8) | (int) (v * 255);
        case 4:
            return ((int) (t * 255) << 16) | ((int) (p * 255) << 8) | (int) (v * 255);
        default:
            return ((int) (v * 255) << 16) | ((int) (p * 255) << 8) | (int) (q * 255);
    }
}

int lerp_color(int rgb1, int rgb2, double t) {
    double h1, s1, v1;
    rgb_to_hsv(rgb1, &h1, &s1, &v1);

    double h2, s2, v2;
    rgb_to_hsv(rgb2, &h2, &s2, &v2);

    double h = lerp_d(h1, h2, t);
    double s = lerp_d(s1, s2, t);
    double v = lerp_d(v1, v2, t);

    return hsv_to_rgb(h, s, v);
}