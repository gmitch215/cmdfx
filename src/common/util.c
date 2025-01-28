#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "cmdfx/util.h"

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
    double r = ((rgb >> 16) & 0xFF) / 255.0;
    double g = ((rgb >> 8) & 0xFF) / 255.0;
    double b = (rgb & 0xFF) / 255.0;

    double min = fmin(fmin(r, g), b);
    double max = fmax(fmax(r, g), b);
    double delta = max - min;

    *v = max;
    *s = (max > 0.0) ? (delta / max) : 0.0;

    if (delta < 0.00001) {
        *h = 0.0;  // No hue
    } else if (r == max) {
        *h = 60.0 * fmod(((g - b) / delta), 6.0);
    } else if (g == max) {
        *h = 60.0 * (((b - r) / delta) + 2.0);
    } else {
        *h = 60.0 * (((r - g) / delta) + 4.0);
    }

    if (*h < 0.0)
        *h += 360.0;
}

int hsv_to_rgb(double h, double s, double v) {
    if (s <= 0.0) {
        int value = (int)(v * 255);
        return (value << 16) | (value << 8) | value;
    }

    h = fmod(h, 360.0);
    if (h < 0.0) h += 360.0;

    double hh = h / 60.0;
    int i = (int)floor(hh);
    double f = hh - i;

    double p = v * (1.0 - s);
    double q = v * (1.0 - (s * f));
    double t = v * (1.0 - (s * (1.0 - f)));

    double r, g, b;
    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }

    // Convert to integer RGB
    int ir = (int)(r * 255.0);
    int ig = (int)(g * 255.0);
    int ib = (int)(b * 255.0);

    return (ir << 16) | (ig << 8) | ib;
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