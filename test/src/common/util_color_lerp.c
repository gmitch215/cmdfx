#include <stdio.h>

#include "cmdfx/util.h"
#include "../test.h"

int main() {
    int r = 0;

    int rgb = 0xFFD700;
    double h, s, v;
    rgb_to_hsv(rgb, &h, &s, &v);
    
    // HSV: 50-51, 100%, 100%
    r |= assert(h >= 50.0 && h <= 51.0);
    r |= assert(s == 1.0);
    r |= assert(v == 1.0);

    int rgb1 = 0xFF0000;
    int rgb2 = 0x00FF00;
    
    r |= assert(lerp_color(rgb1, rgb2, 0.0) == rgb1);
    r |= assert(lerp_color(rgb1, rgb2, 0.25) == 0xFF7F00);
    r |= assert(lerp_color(rgb1, rgb2, 0.5) == 0xFFFF00);
    r |= assert(lerp_color(rgb1, rgb2, 0.75) == 0x7FFF00);
    r |= assert(lerp_color(rgb1, rgb2, 1.0) == rgb2);

    return r;
}