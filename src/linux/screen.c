#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "cmdfx/screen.h"

// Screen API

int Screen_getRGB(int x, int y) {
    const char* framebufferDevice = "/dev/fb0";

    int fb = open(framebufferDevice, O_RDONLY);
    if (fb < 0) {
        perror("Failed to open framebuffer device");
        return -1;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("Failed to get variable screen info");
        close(fb);
        return -1;
    }

    if (ioctl(fb, FBIOGET_FSCREENINFO, &finfo)) {
        perror("Failed to get fixed screen info");
        close(fb);
        return -1;
    }

    long screensize = vinfo.yres_virtual * finfo.line_length;
    unsigned char* framebuffer = (unsigned char*)mmap(0, screensize, PROT_READ, MAP_SHARED, fb, 0);
    if (framebuffer == MAP_FAILED) {
        perror("Failed to map framebuffer device to memory");
        close(fb);
        return -1;
    }

    int location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                   (y + vinfo.yoffset) * finfo.line_length;

    int red = 0, green = 0, blue = 0;
    if (vinfo.bits_per_pixel == 32) {
        blue = framebuffer[location];
        green = framebuffer[location + 1];
        red = framebuffer[location + 2];
    } else if (vinfo.bits_per_pixel == 24) {
        blue = framebuffer[location];
        green = framebuffer[location + 1];
        red = framebuffer[location + 2];
    } else {
        fprintf(stderr, "Unsupported bits per pixel: %d\n", vinfo.bits_per_pixel);
        munmap(framebuffer, screensize);
        close(fb);
        return -1;
    }

    munmap(framebuffer, screensize);
    close(fb);

    return (red << 16) | (green << 8) | blue;
}